#include "PlayerHipDropState.h"
#include"PlayerIdleState.h"
namespace
{
	//ヒップドロップの待機フレーム
	constexpr int kHipDropWaitFrame = 30;
}

void PlayerHipDropState::Enter()
{
	m_player.StopMove();
	m_player.StartHipDrop();
	m_animationController.Play(PlayerAnim::HipDrop);
}

void PlayerHipDropState::Update()
{
	m_count++;
	if (m_count < kHipDropWaitFrame)
	{
		return;
	}

	m_player.HipDropMove();
	if(m_player.IsGround())
	{
		m_player.EndHipDrop();
		m_player.ChangeState(std::make_unique<PlayerIdleState>(m_player, m_animationController));
	}
}

void PlayerHipDropState::Exit()
{
}
