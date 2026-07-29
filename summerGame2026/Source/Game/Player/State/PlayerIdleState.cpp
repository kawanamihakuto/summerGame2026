#include "PlayerIdleState.h"
#include"PlayerMoveState.h"
#include"PlayerJumpState.h"
void PlayerIdleState::Enter()
{
	m_animationController.Play(PlayerAnim::idle);
}

void PlayerIdleState::Update()
{
	if (m_player.GetInputInfo().move.Length() > 0.0f)
	{
		m_player.ChangeState(std::make_unique<PlayerMoveState>(m_player,m_animationController));
	}

	if (m_player.GetInputInfo().isJumpDown)
	{
		m_player.Jump();
	}
}

void PlayerIdleState::Exit()
{
}
