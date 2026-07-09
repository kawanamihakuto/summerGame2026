#include "Hat.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Capture/ICaptureTarget.h"

namespace
{
	constexpr float kSpeed = 10.0f;
	constexpr float kLength = 100.0f;

	constexpr float kSphereRadius = 30.0f;
}

Hat::Hat(int modelHandle,int stageModelHandle,ICaptureTarget& target):
	m_direction({})
{
	m_modelHandle = MV1DuplicateModel(modelHandle);
	m_stageModelHandle = stageModelHandle;

	m_target = target;
	m_targetPos = m_target.
}

Hat::~Hat()
{
	MV1DeleteModel(m_modelHandle);
}

void Hat::Init()
{
}

void Hat::End()
{
}

void Hat::Update()
{
	Vector3 vec = m_targetPos - m_transform.position;
	vec.Normalize();
	m_velocity = vec * kSpeed;

	m_collider->Update(m_transform.position + m_velocity);

	WallCollision(m_stageModelHandle);

	m_transform.Translate(m_velocity);

	MV1SetPosition(m_modelHandle,m_transform.position);
}

void Hat::Draw()
{
	MV1DrawModel(m_modelHandle);
}

const Collider& Hat::GetCollider() const
{
	return *m_collider;
}

const Ray& Hat::GetRay() const
{
	return *m_ray[0];
}

CollisionLayer Hat::GetCollisionLayer() const
{
	return CollisionLayer();
}

CollisionLayer Hat::GetCollisionMask() const
{
	return CollisionLayer();
}

void Hat::OnCollision(ICollider& other)
{
}
