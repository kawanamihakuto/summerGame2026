#pragma once
#include"Engine/Core/StateBase.h"
#include"Engine/Animation/AnimationController.h"
#include"Game/Player/Player.h"
class Player;
class AnimationController;
class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase(Player& player, AnimationController& anim) :
		m_player(player),
		m_animationController(anim)
	{
	}

protected:
	Player& m_player;

	AnimationController& m_animationController;
};

