#pragma once
#include "PlayerStateBase.h"
class PlayerHipDropState : public PlayerStateBase
{
public:
	using PlayerStateBase::PlayerStateBase;

	void Enter() override;
	void Update() override;
	void Exit() override;

private:
	int m_count = 0;
};

