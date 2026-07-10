#include "CaptureManager.h"
#include"Engine/Camera/FollowCamera.h"
CaptureManager::CaptureManager(PlayerController& controller, CameraManager& camreaManager, ICaptureTarget* player):
	m_playerController(controller),
	m_cameraManager(camreaManager),
	m_player(player)
{
}

void CaptureManager::Capture(ICaptureTarget* target)
{
	m_playerController.SetTarget(target);
	m_cameraManager.GetFollowCamera().SetTarget(target);
}

void CaptureManager::Release()
{
	m_playerController.SetTarget(m_player);
	m_cameraManager.GetFollowCamera().SetTarget(m_player);
}
