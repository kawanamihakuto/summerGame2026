#include "PlayerJumpState.h"

void PlayerJumpState::Enter()
{
	m_animationController.Play(PlayerAnim::jump,false);

	m_player.StartJamp();
}

void PlayerJumpState::Update()
{
}

void PlayerJumpState::Exit()
{
}
