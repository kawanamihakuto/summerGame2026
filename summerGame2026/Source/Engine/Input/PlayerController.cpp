#include "PlayerController.h"
#include"InputManager.h"
PlayerController::PlayerController(Hat& hat):
	m_hat(hat)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	auto& input = InputManager::GetInstance();

	if (m_target)
	{
		Vector2 move = input.GetLeftStick();

		m_target->Move(move);
	}

	if (input.IsTriggered("A"))
	{
		m_target->Jump();
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
