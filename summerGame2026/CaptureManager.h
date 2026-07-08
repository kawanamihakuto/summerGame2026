#pragma once

class PlayerController;
class CameraManager;
class Character;
class CaptureManager
{
public:
	CaptureManager(PlayerController& controller,CameraManager& camreaManager,Character& player);

	void Capture(Character* target);

private:
	PlayerController& m_playerController;
	CameraManager& m_cameraManager;
	Character& m_player;
};

