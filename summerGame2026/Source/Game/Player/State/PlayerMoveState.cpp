#include "PlayerMoveState.h"
#include"PlayerIdleState.h"
#include"PlayerJumpState.h"
void PlayerMoveState::Enter()
{
	
}

void PlayerMoveState::Update()
{
	m_player.Move();

	if (m_player.IsRun())
	{
		m_animationController.Play(PlayerAnim::run);
	}
	else
	{
		m_animationController.Play(PlayerAnim::walk);
	}

	if (m_player.GetVelocity().Length() == 0.0f)
	{
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player, m_animationController));
	}

	if (m_player.GetInputInfo().isJumpDown)
	{
		m_player.Jump();
	}
}

void PlayerMoveState::Exit()
{
}
