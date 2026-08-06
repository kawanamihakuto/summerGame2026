#pragma once
#include "Engine/Collision/ICollider.h"
#include"Engine/Core/GameObject.h"
#include"Engine/Core/PreCompiled.h"
class Star : public GameObject , public ICollider
{
public:
	Star(int modelHandle , const Vector3& pos);
	~Star();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	//-----------------------------
	// IColliderの関数
	//-----------------------------
	const Collider& GetCollider()const override;
//	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other, CollisionResult& result) override;

private:
	int m_modelHandle;

	//キャラクターとの当たり判定用コライダー
	std::unique_ptr<Collider>m_bodyCollider;
};

