#pragma once
#include"Engine/Core/GameObject.h"
#include"Engine/Core/ICollider.h"
#include"Engine/Collision/CapsuleCollider.h"
#include"Engine/Collision/Ray.h"

namespace PlayerAnim
{
	const std::wstring idle = L"Armature|Idle";
	const std::wstring run = L"Armature|Run";
	const std::wstring jump = L"Armature|Jump";
};

class AnimationController;
class Player : public GameObject , public ICollider
{
public:
	Player(int playerModel,int stageModel);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;

	Transform* GetTransform();

	Vector3 GetGroundPlayerPos()const;
private:
	int m_modelHandle;

	int m_stageModelHandle;

	std::shared_ptr<AnimationController>m_animationController;

	CapsuleCollider m_capsuleCol;
	std::vector<Ray> m_ray;

	Vector3 m_velocity;

	bool m_isGround;

	Vector3 m_groundPlayerPos;
};

