#include "SphereCollider.h"

SphereCollider::SphereCollider():
	m_info({}, 0.0f)
{
}

void SphereCollider::Init(const Vector3& pos, float radius)
{
	m_info.pos = pos;
	m_info.radius = radius;
}

void SphereCollider::Update(const Vector3& pos)
{
	m_info.pos = pos;
}

void SphereCollider::ReUpdate(const Vector3& pos)
{
	m_info.pos = pos;
}

void SphereCollider::Draw()
{
	DrawSphere3D(m_info.pos.ChangeDxVector(), m_info.radius, 8, 0x00ff00, 0x00ff00, false);
}

void SphereCollider::Hit()
{
}

SphereInfo SphereCollider::GetSphereInfo() const
{
	return m_info;
}

ColliderType SphereCollider::GetType() const
{
	return ColliderType::sphere;
}
