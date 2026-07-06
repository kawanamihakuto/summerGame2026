#include "SceneMain.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Engine/Core/InputManager.h"
#include"Game/Player/Player.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
#include"Stage.h"
#include"ResourceManager.h"
#include"Game/Enemy/CrabEnemy.h"

SceneMain::SceneMain():
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
	m_gameObjectManager->Add(std::make_unique<Player>(resouceManager.GetModel(ModelType::player), resouceManager.GetModel(ModelType::stage),*m_cameraManager));
	m_gameObjectManager->Add(std::make_unique<CrabEnemy>(resouceManager.GetModel(ModelType::crabEnemy), resouceManager.GetModel(ModelType::stage)));
	
	m_gameObjectManager->Init();

	m_cameraManager->AddCamera(std::make_shared<FollowCamera>(m_gameObjectManager->Find<Player>()));
	m_cameraManager->ChangeCamera(CameraName::follow);
	m_stage = std::make_shared<Stage>(resouceManager.GetModel(ModelType::stage));
}

void SceneMain::Update()
{
	m_frameCount++;
	InputManager::GetInstance().Update();

	m_gameObjectManager->Update();
	m_cameraManager->Update();
	m_cameraManager->Apply();
	SetLightDirection(m_cameraManager->GetTransfrom().Forward());
}

void SceneMain::Draw()
{

	m_gameObjectManager->Draw();

	m_stage->Draw();

//	DrawGrid();
	DrawString(0, 0, L"SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", m_frameCount);
}

void SceneMain::DrawGrid()
{
	// 直線の始点と終点
	VECTOR startPos;
	VECTOR endPos;

	for (int z = -300; z <= 300; z += 100)
	{
		startPos = VGet(-300.0f, 0.0f, static_cast<float>(z));
		endPos = VGet(300.0f, 0.0f, static_cast<float>(z));
		DrawLine3D(startPos, endPos, 0xff0000);
	}
	for (int x = -300; x <= 300; x += 100)
	{
		startPos = VGet(static_cast<float>(x), 0.0f, -300.0f);
		endPos = VGet(static_cast<float>(x), 0.0f, 300.0f);
		DrawLine3D(startPos, endPos, 0x0000ff);
	}
}

void SceneMain::End()
{
	m_gameObjectManager->Clear();
	auto& resouceManager = ResourceManager::GetInstance();
	resouceManager.ReleaseResources();
}


