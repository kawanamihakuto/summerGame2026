#include "TitleScene.h"
#include"SceneController.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Core/Application.h"
#include"GameScene.h"
#include"Engine/Core/ResourceManager.h"
#include"Game/Graphics/SkyBox.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/LookAtCamera.h"
#include"Engine/Collision/CollisionManager.h"
#include"Engine/Core/GameObjectManager.h"
#include"Game/Player/TitlePlayer.h"
#include"Game/Item/Hat.h"
namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;

	constexpr Vector3 kCameraPosition = { 0.0f,100.0f,-500.0f };
	constexpr Vector3 kCameraTargetPosition = { 0.0f,100.0f,0.0f };
}

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller)
{
	//フェードイン
	m_update = &TitleScene::FadeInUpdate;
	//フェード用描画
	m_draw = &TitleScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_frame = kFadeInterval;

	//リソースのロード
	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.LoadResources();

	m_cameraManager = std::make_shared<CameraManager>();

	m_cameraManager->AddCamera(std::make_shared<LookAtCamera>(kCameraPosition, kCameraTargetPosition));

	m_cameraManager->ChangeCamera(CameraName::lookat);

	m_collisionManager = std::make_shared<CollisionManager>();

	m_gameObjectManager = std::make_shared<GameObjectManager>(*m_collisionManager);

	m_gameObjectManager->Add(std::make_unique<TitlePlayer>(resouceManager.GetModel(ModelType::player)));

	m_gameObjectManager->Add(std::make_unique<Hat>(resouceManager.GetModel(ModelType::hat), resouceManager.GetModel(ModelType::stage), m_gameObjectManager->Find<TitlePlayer>()));

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
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	(this->*m_update)();
}

void TitleScene::Draw()
{
	(this->*m_draw)();
}

void TitleScene::FadeInUpdate()
{
	if (--m_frame <= 0)
	{
		m_update = &TitleScene::NormalUpdate;
		m_draw = &TitleScene::NormalDraw;
		return;
	}
}

void TitleScene::NormalUpdate()
{
	auto& input = InputManager::GetInstance();
	input.Update();

	m_gameObjectManager->Update();

	m_cameraManager->Update();

	m_cameraManager->Apply();

	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());

	if (input.AnyPressButtonInput())
	{
		m_update = &TitleScene::FadeOutUpdate;
		m_draw = &TitleScene::FadeDraw;
		m_frame = 0;
	}
}

void TitleScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		//シーンの切り替え
		m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
		return;
	}
}

void TitleScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();

	m_skyBox->Draw();

	m_gameObjectManager->Draw();

	int srcW, srcH;

	GetGraphSize(ResourceManager::GetInstance().GetGraph(GraphType::title), &srcW, &srcH);
	//タイトルロゴ
	DrawExtendGraph(static_cast<int>(wsize.w / 2.0f - wsize.w / 5), static_cast<int>(wsize.h / 4.0f - wsize.h / 5),
		static_cast<int>(wsize.w / 2.0f + wsize.w / 5), static_cast<int>(wsize.h / 4.0f + wsize.h / 5), ResourceManager::GetInstance().GetGraph(GraphType::title), true);
	//PressAnyButton
	DrawExtendGraph(static_cast<int>(wsize.w / 2.0f - wsize.w / 5.0f), static_cast<int>(wsize.h / 2.0f - wsize.h / 5.0f + 300),
		static_cast<int>(wsize.w / 2.0f + wsize.w / 5.0f), static_cast<int>(wsize.h / 2.0f + wsize.h / 5.0f + 300), ResourceManager::GetInstance().GetGraph(GraphType::pressAnyButton), true);
#ifdef _DEBUG
	DrawFormatString(static_cast<int>(wsize.w / 2.0f), static_cast<int>(wsize.h / 2.0f), 0xffffff, L"TITLESCENE");
#endif // _DEBUG
}

void TitleScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}
