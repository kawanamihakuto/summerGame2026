#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider():
	m_start({0.0f,0.0f,0.0f}),
	m_end({0.0f,0.0f,0.0f,}),
	m_radius(0.0f),
	m_height(0.0f)
{
}

void CapsuleCollider::Init(const Transform& transform, float radius, float height)
{
	Vector3 pos = transform.GetPosition();

	m_radius = radius;

	m_height = height;

	m_start = pos + Vector3(0.0f, m_radius, 0.0f);
	m_end = pos + Vector3(0.0f, m_height - m_radius, 0.0f);
}

void CapsuleCollider::Update(const Transform& transform)
{
	Vector3 pos = transform.GetPosition();
	m_start = pos + Vector3(0.0f, m_radius, 0.0f);
	m_end = pos + Vector3(0.0f, m_height - m_radius, 0.0f);
}

void CapsuleCollider::Draw()
{
	DrawCapsule3D(m_start.ChangeDxVector(), m_end.ChangeDxVector(),
		m_radius,8,0xffffff,0xffffff,false );
}

ColliderType CapsuleCollider::GetType() const
{
	return ColliderType::capsule;
}
