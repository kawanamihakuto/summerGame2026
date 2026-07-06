#pragma once
#include"Engine/Core/StateBase.h"

class Player;
class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase(Player& player):
		m_player(player)
	{
	}

protected:
	Player& m_player;
};

