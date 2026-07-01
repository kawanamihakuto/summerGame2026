#pragma once
#include"StateBase.h"

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

