#include "ClearScene.h"
#include"SceneController.h"
#include"TitleScene.h"
#include"Engine/Core/Application.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Core/ResourceManager.h"
#include"Game/Graphics/SkyBox.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/LookAtCamera.h"
#include"Engine/Collision/CollisionManager.h"
#include"Engine/Core/GameObjectManager.h"
#include"Game/Player/TitlePlayer.h"
#include"Game/Item/Hat.h"
#include"Engine/Core/SoundManager.h"

namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;

	constexpr Vector3 kCameraPosition = { 0.0f,100.0f,-500.0f };
	constexpr Vector3 kCameraTargetPosition = { 0.0f,100.0f,0.0f };
}

ClearScene::ClearScene(SceneController& controller):
	SceneBase(controller)
{
	//フェードイン
	m_update = &ClearScene::FadeInUpdate;
	//フェード用描画
	m_draw = &ClearScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_frame = kFadeInterval;

	auto& resouceManager = ResourceManager::GetInstance();

	m_cameraManager = std::make_shared<CameraManager>();

	m_cameraManager->AddCamera(std::make_shared<LookAtCamera>(kCameraPosition, kCameraTargetPosition));

	m_cameraManager->ChangeCamera(CameraName::lookat);

	m_collisionManager = std::make_shared<CollisionManager>();

	m_gameObjectManager = std::make_shared<GameObjectManager>(*m_collisionManager);

	m_gameObjectManager->Add(std::make_unique<TitlePlayer>(resouceManager.GetModel(ModelType::player)));

	m_gameObjectManager->Add(std::make_unique<Hat>(resouceManager.GetModel(ModelType::hat), resouceManager.GetModel(ModelType::stageCollider), m_gameObjectManager->Find<TitlePlayer>()));

	m_gameObjectManager->Init();

	m_cameraManager->Update();

	m_cameraManager->Apply();

	//スカイボックス生成
	m_skyBox = std::make_shared<SkyBox>(
		resouceManager.GetGraph(GraphType::skyFront),
		resouceManager.GetGraph(GraphType::skyRight),
		resouceManager.GetGraph(GraphType::skyBack),
		resouceManager.GetGraph(GraphType::skyLeft),
		resouceManager.GetGraph(GraphType::skyUp),
		resouceManager.GetGraph(GraphType::skyBottom)
	);

	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());

	SoundManager::PlayBGM("clear");
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	(this->*m_update)();
}

void ClearScene::Draw()
{
	(this->*m_draw)();
}

void ClearScene::FadeInUpdate()
{
	if (--m_frame <= 0)
	{
		m_update = &ClearScene::NormalUpdate;
		m_draw = &ClearScene::NormalDraw;
		return;
	}
}

void ClearScene::NormalUpdate()
{
	auto& input = InputManager::GetInstance();
	input.Update();

	m_gameObjectManager->Update();

	m_cameraManager->Update();

	m_cameraManager->Apply();

	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());

	if (input.AnyPressButtonInput())
	{
		m_update = &ClearScene::FadeOutUpdate;
		m_draw = &ClearScene::FadeDraw;
		m_frame = 0;
		SoundManager::PlaySE("pressAnyButton", false);
		SoundManager::FadeOutBGM(kFadeInterval);
	}
}

void ClearScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		//シーンの切り替え
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
}

void ClearScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();

	auto& resouceManager = ResourceManager::GetInstance();

	m_skyBox->Draw();

	m_gameObjectManager->Draw();

	int srcW, srcH;

	GetGraphSize(ResourceManager::GetInstance().GetGraph(GraphType::title), &srcW, &srcH);
	//クリア
	DrawExtendGraph(static_cast<int>(wsize.w / 2.0f - wsize.w / 3), static_cast<int>(wsize.h / 4.0f - wsize.h / 3),
		static_cast<int>(wsize.w / 2.0f + wsize.w / 3), static_cast<int>(wsize.h / 4.0f + wsize.h / 3), ResourceManager::GetInstance().GetGraph(GraphType::clear), true);
	//PressAnyButton
	DrawExtendGraph(static_cast<int>(wsize.w / 2.0f - wsize.w / 5.0f), static_cast<int>(wsize.h / 2.0f - wsize.h / 5.0f + 300),
		static_cast<int>(wsize.w / 2.0f + wsize.w / 5.0f), static_cast<int>(wsize.h / 2.0f + wsize.h / 5.0f + 300), ResourceManager::GetInstance().GetGraph(GraphType::pressAnyButton), true);

#ifdef _DEBUG
	DrawFormatString(static_cast<int>(wsize.w / 2.0f), static_cast<int>(wsize.h / 2.0f), 0xffffff, L"ClearScene");
#endif // _DEBUG
}

void ClearScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
	if (m_update == &ClearScene::FadeInUpdate)
	{
		DrawBox(0, 0, wsize.w, wsize.h, 0xffffff, true);
	}
	else if (m_update == &ClearScene::FadeOutUpdate)
	{
		DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}
