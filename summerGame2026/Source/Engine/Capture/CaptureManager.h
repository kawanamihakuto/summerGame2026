#pragma once
#include"Engine/Input/PlayerController.h"
#include"Engine/Camera/CameraManager.h"
#include"ICaptureTarget.h"

class CaptureManager
{
public:
	CaptureManager(Player* player,Hat* hat,CameraManager& camreaManager,ICaptureTarget* target);

	void Capture(ICaptureTarget* target);
	void Release();

	ICaptureTarget* GetTarget();
private:
	Player* m_player;
	Hat* m_hat;
	CameraManager& m_cameraManager;
	ICaptureTarget* m_currentTarget = nullptr;
};

