#include "PlayerController.h"
#include"InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
#include"Engine/Capture/CaptureManager.h"
PlayerController::PlayerController(Hat* hat, Player* player, CaptureManager& captureManager):
	m_hat(hat),
	m_player(player),
	m_captureManager(captureManager),
	m_target(player)
{
	
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	auto& input = InputManager::GetInstance();
	Vector2 move = input.GetLeftStick();

	m_target = m_target->GetControllTarget();

	if (m_target)
	{
		m_target->Move(move);
	}

	if (input.IsTriggered("A"))
	{
		m_target->Jump();
	}

	if (input.IsTriggered("X"))
	{
		if (m_player->IsActive())
		{
			m_hat->Throw(m_player->GetLastMoveDirection());
		}
	}

	if (input.IsTriggered("Y"))
	{
		if (!m_player->IsActive())
		{
			m_captureManager.Release();
			SetTarget(m_player);
		}
	}

	if (m_hat->GetCaptureFlag())
	{
		m_captureManager.Capture(m_hat->GetCaptureTarget());
		SetTarget(m_hat->GetCaptureTarget());
		m_hat->ResetCaptureFlag();
	}
}

void PlayerController::SetTarget(ICaptureTarget* target)
{
	if (m_target)
	{
		m_target->ExitControll();
	}
	m_target = target->GetControllTarget();
	m_target->Controll();
}