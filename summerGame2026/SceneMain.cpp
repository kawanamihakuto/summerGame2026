#include "SceneMain.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Engine/Input/InputManager.h"
#include"Game/Player/Player.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
#include"Stage.h"
#include"ResourceManager.h"
#include"Game/Enemy/CrabEnemy.h"
#include"SkyBox.h"
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
	SetUseZBuffer3D(true);	// Zバッファを使います
	SetWriteZBuffer3D(true);	// 描画する物体はZバッファにも距離を書き込む

	// カメラの設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 300.0f, -700.0f), VGet(0.0f, 0.0f, 0.0f));
	SetupCamera_Perspective(DX_PI_F / 3.0f);
	SetCameraNearFar(50.0f, 3000.0f);

	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.LoadResources();

	m_collisionManager = std::make_shared<CollisionManager>();
	m_cameraManager = std::make_shared<CameraManager>();

	m_gameObjectManager = std::make_shared<GameObjectManager>(*m_collisionManager);
	m_gameObjectManager->Add(std::make_unique<Player>(resouceManager.GetModel(ModelType::player), resouceManager.GetModel(ModelType::stage), *m_cameraManager));
	m_gameObjectManager->Add(std::make_unique<Hat>(resouceManager.GetModel(ModelType::hat), resouceManager.GetModel(ModelType::stage), m_gameObjectManager->Find<Player>()));

	m_cameraManager->AddCamera(std::make_shared<FollowCamera>(m_gameObjectManager->Find<Player>()));
	m_cameraManager->ChangeCamera(CameraName::follow);
	m_stage = std::make_shared<Stage>(resouceManager.GetModel(ModelType::stage));
	int test = MV1SetupCollInfo(resouceManager.GetModel(ModelType::stage), -1, 8, 8, 8);

	m_captureManager = std::make_shared<CaptureManager>(
		m_gameObjectManager->Find<Player>(),
		m_gameObjectManager->Find<Hat>(),
		*m_cameraManager,
		m_gameObjectManager->Find<Player>());

	m_gameObjectManager->Add(std::make_unique<CrabEnemy>(resouceManager.GetModel(ModelType::crabEnemy), resouceManager.GetModel(ModelType::stage), *m_cameraManager, *m_captureManager, Vector3{0.0f,-200.0f,0.0f}));
	m_gameObjectManager->Add(std::make_unique<CrabEnemy>(resouceManager.GetModel(ModelType::crabEnemy), resouceManager.GetModel(ModelType::stage), *m_cameraManager, *m_captureManager, Vector3{ 100.0f,-200.0f,0.0f }));
	m_gameObjectManager->Add(std::make_unique<CrabEnemy>(resouceManager.GetModel(ModelType::crabEnemy), resouceManager.GetModel(ModelType::stage), *m_cameraManager, *m_captureManager, Vector3{ 0.0f,-200.0f,100.0f }));
	
	m_gameObjectManager->Init();
	
	m_playerController = std::make_shared<PlayerController>(m_gameObjectManager->Find<Hat>(), m_gameObjectManager->Find<Player>(), *m_captureManager);

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

void SceneMain::Update()
{
	m_frameCount++;
	auto& input = InputManager::GetInstance();
	input.Update();

	m_playerController->Update();

	m_gameObjectManager->Update();

	m_collisionManager->CheckAllCollisions();

	m_cameraManager->Update();
	m_cameraManager->Apply();

	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());

	SetLightDirection(m_cameraManager->GetTransfrom().Forward());

	if (input.GetInstance().IsTriggered("SELECT"))
	{
		auto& resouceManager = ResourceManager::GetInstance();
		m_gameObjectManager->Add(std::make_unique<CrabEnemy>(resouceManager.GetModel(ModelType::crabEnemy), resouceManager.GetModel(ModelType::stage), *m_cameraManager, *m_captureManager, Vector3{ 0.0f,-200.0f,100.0f }));
	}
}

void SceneMain::Draw()
{
	m_skyBox->Draw();

	m_gameObjectManager->Draw();

	m_stage->Draw();

#ifdef _DEBUG
	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", m_frameCount);
#endif // _DEBUG
}

void SceneMain::End()
{
	m_gameObjectManager->Clear();
	m_collisionManager->ClearObjects();
	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.ReleaseResources();
}


