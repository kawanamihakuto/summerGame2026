#include "SceneMain.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Engine/Core/InputManager.h"
#include"Game/Player/Player.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
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
	SetCameraNearFar(200.0f, 1500.0f);

	auto& gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager.Add(std::make_unique<Player>());
	gameObjectManager.Init();

	auto& cameraManager = CameraManager::GetInstance();
	cameraManager.Init(std::make_unique<FollowCamera>());
}

void SceneMain::Update()
{
	m_frameCount++;
	InputManager::GetInstance().Update();
	auto& gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager.Update();
	auto& cameraManager = CameraManager::GetInstance();
	cameraManager.Update();
	cameraManager.Apply();
}

void SceneMain::Draw()
{
	auto& gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager.Draw();
	DrawGrid();
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
	auto& gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager.Clear();
}


