#include "CrabEnemy.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Animation/AnimationController.h"

namespace
{
	
}

CrabEnemy::CrabEnemy(int enemyModel, int stageModel)
{
	m_modelHandle = MV1DuplicateModel(enemyModel);
	m_stageModelHandle = stageModel;
}

CrabEnemy::~CrabEnemy()
{
	MV1DeleteModel(m_modelHandle);
}

void CrabEnemy::Init()
{
	m_sphereCol.Init(m_transform.GetPosition(), 50.0f);

	m_animationController = std::make_shared<AnimationController>(m_modelHandle);
	m_animationController->AddAnimation(CrabEnemyAnim::idle);
	m_animationController->AddAnimation(CrabEnemyAnim::walk);
	m_animationController->AddAnimation(CrabEnemyAnim::jump);
	m_animationController->Play(CrabEnemyAnim::idle);
}

void CrabEnemy::End()
{
}

void CrabEnemy::Update()
{
	m_animationController->Update();
}

void CrabEnemy::Draw()
{
	MV1DrawModel(m_modelHandle);
}

const Collider& CrabEnemy::GetCollider() const
{
	return m_sphereCol;
}

const Ray& CrabEnemy::GetRay() const
{
	return m_ray[0];
}

CollisionLayer CrabEnemy::GetCollisionLayer() const
{
	return CollisionLayers::kEnemy;
}

CollisionLayer CrabEnemy::GetCollisionMask() const
{
	return CollisionLayers::kPlayer;
}

void CrabEnemy::OnCollision(ICollider& other)
{

}
