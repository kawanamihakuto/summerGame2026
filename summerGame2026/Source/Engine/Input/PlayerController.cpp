#include "PlayerController.h"
#include"InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
PlayerController::PlayerController(Hat* hat, Player* player,CameraManager& cameraManager) :
	m_hat(hat),
	m_player(player),
	m_cameraManager(cameraManager)
{
	SetTarget(m_player);
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	auto& input = InputManager::GetInstance();
	Vector2 move = input.GetLeftStick();

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
		m_hat->Throw(m_player->GetTransform()->Forward());
	}

	if (m_hat->GetCaptureFlag())
	{
		Capture(m_hat->GetCaptureTarget());
		m_hat->ResetCaptureFlag();
	}
}

void PlayerController::SetTarget(IControllable* target)
{
	if (m_target)
	{
		m_target->Controll(false);
	}
	m_target = target;
	m_target->Controll(true);
}

void PlayerController::Capture(ICaptureTarget* target)
{
	m_cameraManager.GetFollowCamera().SetTarget(target);
	SetTarget(target);
}

void PlayerController::Release()
{
	m_cameraManager.GetFollowCamera().SetTarget(m_player);
	SetTarget(m_player);
}
