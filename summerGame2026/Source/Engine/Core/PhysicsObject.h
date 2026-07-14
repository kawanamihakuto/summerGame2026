#pragma once
#include "GameObject.h"
#include"Precompiled.h"
#include"Engine/Collision/Collider.h"
#include"Engine/Collision/Ray.h"

class PhysicsObject : public GameObject
{
public:
	void ResolveWallVelocity(int stageModelHandle);
	void ResolveWallPosition(int stageModelHandle);
	void GroundCollision(int stageModelHandle);
	void Gravity();

	virtual void Init()override abstract;
	virtual void End()override abstract;
	virtual void Update()override abstract;
	virtual void Draw()override abstract;

protected:
	Vector3 m_velocity = {};

	std::unique_ptr<Collider> m_collider;

	std::vector<std::unique_ptr<Ray>> m_ray;

	bool m_isGround = false;
};

