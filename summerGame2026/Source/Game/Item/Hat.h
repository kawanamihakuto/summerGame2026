#pragma once
#include "Engine/Core/PhysicsObject.h"
#include"Engine/Collision/ICollider.h"

class ICaptureTarget;
class Hat : public PhysicsObject, public ICollider
{
public:
	Hat(int modelHandle, int stageModelHandle,ICaptureTarget& target);
	~Hat();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	//-----------------------------
	// IColliderの関数
	//-----------------------------
	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other) override;

private:
	int m_modelHandle;
	int m_stageModelHandle;

	Vector3 m_direction;

	Vector3 m_targetPos;

	Vector3 m_playerPos;

	ICaptureTarget& m_target;
};