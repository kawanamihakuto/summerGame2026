#pragma once
#include "PlayerStateBase.h"
class PlayerJumpState : public PlayerStateBase
{ 
public:
	using PlayerStateBase::PlayerStateBase;

	void Enter() override;
	void Update() override;
	void Exit() override;
};

