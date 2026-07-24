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
	//ターゲット適用
	m_currentTarget = target;
	//カメラに通知
	m_cameraManager.GetFollowCamera().SetTarget(target->GetHatAndCameraTarget());
}

void CaptureManager::Release()
{
	//プレイヤーのキャプチャー解除時アクション
	m_player->CaptureReleaseAction(m_currentTarget->GetHatMatrix().GetTranslation());
	//ターゲットをプレイヤーに戻す
	m_currentTarget = m_player;
	//カメラに通知
	m_cameraManager.GetFollowCamera().SetTarget(m_player);
	//帽子に通知
	m_hat->SetTarget(m_player);
}

ICaptureTarget* CaptureManager::GetTarget()
{
	//現在のターゲットだよ
	return m_currentTarget;
}

GameObject* CaptureManager::GetTargetObject()
{
	return m_currentTarget->GetGameObject();
}
