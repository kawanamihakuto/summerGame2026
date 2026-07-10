#pragma once
#include"Engine/Input/PlayerController.h"
#include"Engine/Camera/CameraManager.h"
#include"ICaptureTarget.h"

class CaptureManager
{
public:
	CaptureManager(PlayerController& controller,CameraManager& camreaManager,ICaptureTarget* player);

	void Capture(ICaptureTarget* target);

	void Release();

private:
	PlayerController& m_playerController;
	CameraManager& m_cameraManager;
	ICaptureTarget* m_player = nullptr;
};

