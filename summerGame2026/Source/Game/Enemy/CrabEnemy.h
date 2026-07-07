#pragma once
#include "Engine/Core/GameObject.h"
#include"Engine/Core/Character.h"
#include "Engine/Core/ICollider.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Collision/Ray.h"

namespace CrabEnemyAnim
{
	const std::wstring idle = L"MonsterArmature|Idle";
	const std::wstring walk = L"MonsterArmature|Walk";
	const std::wstring jump = L"MonsterArmature|Jump";
};

class AnimationController;
class CrabEnemy : public Character, public ICollider
{
public:
	CrabEnemy(int enemyModel, int stageModel,CameraManager& camrea);
	~CrabEnemy();
	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;
	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other) override;

private:
	int m_modelHandle;
	int m_stageModelHandle;

	std::shared_ptr<AnimationController>m_animationController;

	SphereCollider m_sphereCol;
	std::vector<Ray> m_ray;
};

