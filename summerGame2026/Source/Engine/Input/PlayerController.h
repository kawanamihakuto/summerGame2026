#pragma once
#include"IControllable.h"
#include"Game/Item/Hat.h"
#include"Game/Player/Player.h"

class PlayerController
{
public:
	PlayerController(Hat* hat,Player* player,CameraManager& cameraManager);
	~PlayerController();

	void Update();

	void SetTarget(ICaptureTarget* target);

	void Capture(ICaptureTarget* target);

	void Release();

private:
	ICaptureTarget* m_target = nullptr;
	Hat* m_hat;
	Player* m_player;
	CameraManager& m_cameraManager;
};

