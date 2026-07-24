#include "SceneMain.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Engine/Core/ResourceManager.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Input/PlayerController.h"
#include"Engine/Capture/CaptureManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
#include"Game/Stage/Stage.h"
#include"Game/Player/Player.h"
#include"Game/Enemy/CrabEnemy.h"
#include"Game/Item/Hat.h"
#include"Game/Graphics/SkyBox.h"

namespace
{
	//カメラ設定
	constexpr Vector3 kCameraPosition = { 0.0f, 300.0f, -700.0f };
	constexpr Vector3 kCameraTarget = { 0.0f, 0.0f, 0.0f };
	constexpr float kFov = DX_PI_F / 3.0f;
	constexpr float kCameraNear = 50.0f;
	constexpr float kCameraFar = 6000.0f;

	//敵のスポーン位置(仮)
	constexpr Vector3 kCrabSpawn1 = { 0.0f, -200.0f, 0.0f };
	constexpr Vector3 kCrabSpawn2 = { 100.0f, -200.0f, 0.0f };
	constexpr Vector3 kCrabSpawn3 = { 0.0f, -200.0f, 100.0f };
}

SceneMain::SceneMain() :
	m_frameCount(0)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// Zバッファの設定
	SetUseZBuffer3D(true);	
	SetWriteZBuffer3D(true);

	// カメラの設定
	SetCameraPositionAndTarget_UpVecY(kCameraPosition, kCameraTarget);
	SetupCamera_Perspective(kFov);
	SetCameraNearFar(kCameraNear, kCameraFar);

	//リソースのロード
	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.LoadResources();
	
	//コリジョンマネージャー生成
	m_collisionManager = std::make_shared<CollisionManager>();
	//カメラマネージャー生成
	m_cameraManager = std::make_shared<CameraManager>();
	//ゲームオブジェクトマネージャー生成
	m_gameObjectManager = std::make_shared<GameObjectManager>(*m_collisionManager);
	//プレイヤー生成
	m_gameObjectManager->Add(std::make_unique<Player>(resouceManager.GetModel(ModelType::player), resouceManager.GetModel(ModelType::stage), *m_cameraManager));
	//帽子生成
	m_gameObjectManager->Add(std::make_unique<Hat>(resouceManager.GetModel(ModelType::hat), resouceManager.GetModel(ModelType::stage), m_gameObjectManager->Find<Player>()));
	//フォローカメラ追加
	m_cameraManager->AddCamera(std::make_shared<FollowCamera>(m_gameObjectManager->Find<Player>()));
	//現在のカメラをセット
	m_cameraManager->ChangeCamera(CameraName::follow);
	//ステージ
	m_stage = std::make_shared<Stage>(resouceManager.GetModel(ModelType::stage));
	//ステージとの当たり判定をするためのセットアップ
	MV1SetupCollInfo(resouceManager.GetModel(ModelType::stage), -1, 8, 8, 8);
	//キャプチャーマネージャー生成
	m_captureManager = std::make_shared<CaptureManager>(
		m_gameObjectManager->Find<Player>(),
		m_gameObjectManager->Find<Hat>(),
		*m_cameraManager,
		m_gameObjectManager->Find<Player>());
	//ゲームオブジェクト全体を初期化
	m_gameObjectManager->Init();
	//プレイヤーコントローラー生成
	m_playerController = std::make_shared<PlayerController>(m_gameObjectManager->Find<Hat>(), m_gameObjectManager->Find<Player>(), *m_captureManager);
	//スカイボックス生成
	m_skyBox = std::make_shared<SkyBox>(
		resouceManager.GetGraph(GraphType::skyFront),
		resouceManager.GetGraph(GraphType::skyRight),
		resouceManager.GetGraph(GraphType::skyBack),
		resouceManager.GetGraph(GraphType::skyLeft),
		resouceManager.GetGraph(GraphType::skyUp),
		resouceManager.GetGraph(GraphType::skyBottom)
	);
	//カメラポジションをセット
	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());
}

void SceneMain::Update()
{
	m_frameCount++;

	//入力取得用
	auto& input = InputManager::GetInstance();
	input.Update();

	//プレイヤーコントローラー更新
	m_playerController->Update();
	//ゲームオブジェクト更新
	m_gameObjectManager->Update();
	//全体の当たり判定
	m_collisionManager->CheckAllCollisions();
	//カメラ更新
	m_cameraManager->Update();
	//DxLibに反映
	m_cameraManager->Apply();
	//スカイボックスにカメラポジションセット
	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());
	//ライトの方向を適用(仮)
	SetLightDirection(m_cameraManager->GetTransfrom().Forward());

	//敵生成
	if (InputManager::GetInstance().IsPressed("SELECT") && m_frameCount % 20 == 0)
	{
		auto& resouceManager = ResourceManager::GetInstance();
		m_gameObjectManager->Add(std::make_unique<CrabEnemy>(resouceManager.GetModel(ModelType::crabEnemy), resouceManager.GetModel(ModelType::stage), *m_cameraManager, *m_captureManager, Vector3{ 600.0f,100.0f,0.0f }));
	}
}

void SceneMain::Draw()
{
	//スカイボックス描画
	m_skyBox->Draw();
	//ゲームオブジェクト描画
	m_gameObjectManager->Draw();
	//ステージ描画
	m_stage->Draw();

#ifdef _DEBUG
	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", m_frameCount);
#endif // _DEBUG
}

void SceneMain::End()
{
	//ゲームオブジェクトクリア
	m_gameObjectManager->Clear();
	//当たり判定オブジェクトクリア
	m_collisionManager->ClearObjects();
	//リソースの解放
	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.ReleaseResources();
}


