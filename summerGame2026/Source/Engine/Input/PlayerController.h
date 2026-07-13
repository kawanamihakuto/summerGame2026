#pragma once
#include"IControllable.h"
#include"Game/Item/Hat.h"
#include"Game/Player/Player.h"

class CaptureManager;
class PlayerController
{
public:
	PlayerController(Hat* hat,Player* player,CaptureManager& captureManager);
	~PlayerController();

	void Update();

	void SetTarget(ICaptureTarget* target);

private:
	ICaptureTarget* m_target = nullptr;
	Hat* m_hat;
	Player* m_player;
	
	CaptureManager& m_captureManager;
};

