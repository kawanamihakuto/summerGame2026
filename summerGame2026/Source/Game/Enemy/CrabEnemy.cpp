#include "CrabEnemy.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Quaternion.h"
#include"Engine/Collision/SphereCollider.h"
namespace
{
	constexpr Vector3 kSphereOffset = { 0.0f,50.0f,0.0f };
	constexpr float kSpeed = 2.0f;
	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());
	constexpr Vector3 kScale = { 0.5f,0.5f,0.5f };
	constexpr float kRadius = 50.0f;
}

CrabEnemy::CrabEnemy(int enemyModel, int stageModel, CameraManager& camera) :
	Character(camera)
{
	m_modelHandle = MV1DuplicateModel(enemyModel);
	m_stageModelHandle = stageModel;

	m_collider = std::make_unique<SphereCollider>(m_transform.position, kRadius);
}

CrabEnemy::~CrabEnemy()
{
	MV1DeleteModel(m_modelHandle);
}

void CrabEnemy::Init()
{
	m_animationController = std::make_shared<AnimationController>(m_modelHandle);
	m_animationController->AddAnimation(CrabEnemyAnim::idle);
	m_animationController->AddAnimation(CrabEnemyAnim::walk);
	m_animationController->AddAnimation(CrabEnemyAnim::jump);
	m_animationController->Play(CrabEnemyAnim::idle);

	m_transform.SetPosition({ 0.0f,-200.0f,0.0f });

	MV1SetScale(m_modelHandle,kScale);
}

void CrabEnemy::End()
{
}

void CrabEnemy::Update()
{
	if (m_target != nullptr)
	{
		Vector3 vec = m_target->GetPosition() - m_transform.position;
		vec.y = 0.0f;
		if (vec.Length() > 1.0f)
		{
			m_velocity = vec;
			m_velocity.Normalize();
		}
		else
		{
			m_velocity = Vector3::Zero();
		}
	}

	m_velocity.x *= kSpeed;
	m_velocity.y = 0.0f;
	m_velocity.z *= kSpeed;

	m_transform.Translate(m_velocity);
	UpdateRotate(kModelRotationOffset);

	MV1SetPosition(m_modelHandle, m_transform.GetPosition());
	MV1SetRotationMatrix(m_modelHandle, m_transform.GetRotationMatrix().ChangeDxMat());
	m_collider->Update(m_transform.position + kSphereOffset);
	m_animationController->Update();
}

void CrabEnemy::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_collider->Draw();
#endif // _DEBUG

}

void CrabEnemy::SetTarget(const ITarget* target)
{
	m_target = target;
}

const Collider& CrabEnemy::GetCollider() const
{
	return *m_collider;
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
		DrawFormatString(16, 316, 0xffffff, L"プレイヤーにヒット");
#endif // _DEBUG
	}
}