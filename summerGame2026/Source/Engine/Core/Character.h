#pragma once
#include "GameObject.h"
#include"Engine/Core/Precompiled.h"

class CameraManager;
class Collider;
class Ray;
class Character : public GameObject
{
public:
	Character(CameraManager& cameraManager);
	void InputMove(float speed);
	void UpdateMove();
	void UpdateRotate(Quaternion rotationOffset);
	void Gravity();

	void WallCollision(int stageModelHandle);

	void GroundCollision(int stageModelHandle);

protected:
	Vector3 m_velocity{};

	Vector3 m_moveInput{};

	CameraManager& m_cameraManager;

	std::unique_ptr<Collider> m_collider;

	std::vector<std::unique_ptr<Ray>> m_ray;

	bool m_isGround;
};