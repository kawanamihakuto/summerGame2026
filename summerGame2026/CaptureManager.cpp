#include "CaptureManager.h"
#include"Engine/Input/PlayerController.h"

CaptureManager::CaptureManager(PlayerController& controller, CameraManager& camreaManager, Character& player):
	m_playerController(controller),
	m_cameraManager(camreaManager),
	m_player(player)
{
}

void CaptureManager::Capture(Character* target)
{
//	m_playerController.SetTarget(target);


}
