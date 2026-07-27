#include "PlayerIdleState.h"

void PlayerIdleState::Enter()
{
	m_animationController.Play(PlayerAnim::idle);
}

void PlayerIdleState::Update()
{
}

void PlayerIdleState::Exit()
{
}
