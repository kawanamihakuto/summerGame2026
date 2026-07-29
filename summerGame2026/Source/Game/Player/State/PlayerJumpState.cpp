#include "PlayerJumpState.h"
#include"PlayerIdleState.h"
#include"PlayerMoveState.h"
void PlayerJumpState::Enter()
{
	m_animationController.Play(PlayerAnim::jump,false);

	m_player.StartJamp();
}

void PlayerJumpState::Update()
{
	m_player.Move();

	if (m_player.GetInputInfo().isJumpUp)
	{
		m_player.JumpEnd();
	}

	if (m_player.IsGround())
	{
		if (m_player.GetVelocity().Length() == 0.0f)
		{
			m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player, m_animationController));
		}
		else
		{
			m_player.ChangeState(std::make_unique<PlayerMoveState>(m_player, m_animationController));
		}
	}
}

void PlayerJumpState::Exit()
{
}
