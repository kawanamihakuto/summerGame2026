#include "CrabEnemy.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Animation/AnimationController.h"

namespace
{
	constexpr Vector3 kSphereOffset = { 0.0f,50.0f,0.0f };
}

CrabEnemy::CrabEnemy(int enemyModel, int stageModel, CameraManager& camera) :
	Character(camera)
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

	m_transform.SetPosition({0.0f,-50.0f,0.0f});
	
	MV1SetScale(m_modelHandle, {0.5f,0.5f,0.5f});
}

void CrabEnemy::End()
{
}

void CrabEnemy::Update()
{
	MV1SetPosition(m_modelHandle, m_transform.GetPosition());
	m_sphereCol.Update(m_transform.position + kSphereOffset);
	m_animationController->Update();
}

void CrabEnemy::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_sphereCol.Draw();
#endif // _DEBUG

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
	if (other.GetCollisionLayer() == CollisionLayers::kPlayer)
	{
#ifdef _DEBUG
		DrawFormatString(16,316,0xffffff,L"プレイヤーにヒット");
#endif // _DEBUG
	}
}