#pragma once
#include "Engine/Core/PhysicsObject.h"
#include"Engine/Collision/ICollider.h"
class Hat : public PhysicsObject , public ICollider
{
public:
	Hat(const Vector3& pos,const Vector3& dir);
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

	Vector3 m_direction;
};