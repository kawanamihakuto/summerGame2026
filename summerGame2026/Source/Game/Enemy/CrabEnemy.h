#pragma once
#include "Engine/Core/Character.h"
#include "Engine/Collision/ICollider.h"
#include "Engine/Collision/Ray.h"
#include"Engine/AI/ITarget.h"
#include"Engine/Input/IControllable.h"
namespace CrabEnemyAnim
{
	const std::wstring idle = L"MonsterArmature|Idle";
	const std::wstring walk = L"MonsterArmature|Walk";
	const std::wstring jump = L"MonsterArmature|Jump";
};

class AnimationController;
class CrabEnemy : public Character, public ICollider,public IControllable
{
public:
	CrabEnemy(int enemyModel, int stageModel,CameraManager& camrea);
	~CrabEnemy();
	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	void SetTarget(const ITarget* target);

	//------------------------------
	// IColliderの関数
	//------------------------------
	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other) override;

	//------------------------------
	// IControllableの関数
	//------------------------------
	void Move(const Vector2& input)override;
	void Jump()override;

private:
	const ITarget* m_target = nullptr;

	int m_modelHandle;
	int m_stageModelHandle;

	std::shared_ptr<AnimationController>m_animationController;
};

