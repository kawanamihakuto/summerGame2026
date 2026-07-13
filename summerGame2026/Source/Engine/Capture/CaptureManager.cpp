#include "CaptureManager.h"
#include"Engine/Camera/FollowCamera.h"

CaptureManager::CaptureManager(Player* player, Hat* hat,CameraManager& camreaManager, ICaptureTarget* target):
	m_player(player),
	m_hat(hat),
	m_cameraManager(camreaManager),
	m_currentTarget(target)
{
}

void CaptureManager::Capture(ICaptureTarget* target)
{
	m_currentTarget = target;
	m_cameraManager.GetFollowCamera().SetTarget(target);
	m_player->SetIsActive(false);
}

void CaptureManager::Release()
{
	m_player->CaptureReleaseAction(m_currentTarget->GetHatMatrix().GetTranslation());
	m_currentTarget = m_player;
	m_cameraManager.GetFollowCamera().SetTarget(m_player);
	m_hat->SetTarget(m_player);
	m_player->SetIsActive(true);	
}

ICaptureTarget* CaptureManager::GetTarget()
{
	return m_currentTarget;
}
