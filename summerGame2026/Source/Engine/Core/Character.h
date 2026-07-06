#pragma once
#include "GameObject.h"

class CameraManager;
class Character : public GameObject
{
public:
	Character(CameraManager& cameraManager);
	void InputMove(float speed);
	void UpdateMove();
	void UpdateRotate(Quaternion rotationOffset);

protected:
	Vector3 m_velocity{};

	Vector3 m_moveInput{};

	CameraManager& m_cameraManager;
};