#include "PlayerStunnedState.h"
#include"PlayerMoveState.h"
#include"PlayerIdleState.h"
#include"Engine/Core/SoundManager.h"
void PlayerStunnedState::Enter()
{
	SoundManager::PlaySE("playerHit", false);
	m_animationController.Play(PlayerAnim::stunned, false);
	m_player.StopHorizontalMove();
}

void PlayerStunnedState::Update()
{
	m_player.Move();
	m_player.StopHorizontalMove();

	if (m_animationController.IsEnd())
	{
		if (m_player.GetInputInfo().move.Length() > 0.0f)
		{
			m_player.ChangeState(std::make_unique<PlayerMoveState>(m_player, m_animationController));
		}
		else if (m_player.GetInputInfo().move.Length() == 0.0f)
		{
			m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player, m_animationController));
		}
	}
}

void PlayerStunnedState::Exit()
{
}
