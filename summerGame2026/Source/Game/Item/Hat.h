#pragma once
#include "Engine/Core/PhysicsObject.h"
#include"Engine/Collision/ICollider.h"

enum class HatState
{
	have,
	go,
	wait,
	back
};

class ICaptureTarget;
class Hat : public PhysicsObject, public ICollider
{
public:
	Hat(int modelHandle, int stageModelHandle, ICaptureTarget* target);
	~Hat();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	void Throw(const Vector3& dir);

	void SetTarget(ICaptureTarget* target);

	ICaptureTarget* GetCaptureTarget();

	bool GetCaptureFlag() { return m_CaptureFlag; }

	void ResetCaptureFlag() { m_CaptureFlag = false; }

	//-----------------------------
	// IColliderの関数
	//-----------------------------
	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other, CollisionResult& result) override;
	
private:
	int m_modelHandle;
	int m_stageModelHandle;

	Vector3 m_endPos;

	Vector3 m_targetPos;

	ICaptureTarget* m_target;

	HatState m_state;

	Vector3 m_direction;

	int m_count;

	bool m_CaptureFlag;
};