#pragma once
#include"PhysicsObject.h"
#include"Engine/Core/Precompiled.h"

class CameraManager;
class Collider;
class Ray;
class Character : public PhysicsObject
{
public:
	Character(CameraManager& cameraManager);
	virtual ~Character() = default;
	void InputMove(float speed);
	void UpdateMove();
	void UpdateRotate(Quaternion rotationOffset);

	Vector3 GetMoveInput() const { return m_moveInput; }

protected:
	Vector3 m_moveInput{};
	Vector3 m_lastMoveInput{};
	CameraManager& m_cameraManager;
	int m_hp;

	bool m_isInvincible;

	int m_InvincibleFrameCount;
};