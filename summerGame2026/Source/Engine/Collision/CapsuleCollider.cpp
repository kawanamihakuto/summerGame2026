#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider():
	m_info({}, {},0.0f,0.0f),
	m_col(0x00ff00)
{
}

void CapsuleCollider::Init(const Vector3& pos, float radius, float height)
{
	Vector3 position = pos;

	m_info.radius = radius;

	m_info.height = height;

	m_info.start = position + Vector3(0.0f, m_info.radius, 0.0f);
	m_info.end = position + Vector3(0.0f, m_info.height - m_info.radius, 0.0f);
}

void CapsuleCollider::Update(const Vector3& pos)
{
	Vector3 position = pos;
	m_info.start = position + Vector3(0.0f, m_info.radius, 0.0f);
	m_info.end = position + Vector3(0.0f, m_info.height - m_info.radius, 0.0f);

	m_col = 0x00ff00;
}

void CapsuleCollider::ReUpdate(const Vector3& pos)
{
	Vector3 position = pos;
	m_info.start = position + Vector3(0.0f, m_info.radius, 0.0f);
	m_info.end = position + Vector3(0.0f, m_info.height - m_info.radius, 0.0f);
}

void CapsuleCollider::Draw()
{
	DrawCapsule3D(m_info.start.ChangeDxVector(), m_info.end.ChangeDxVector(),
		m_info.radius,8,m_col,m_col,false );
}

void CapsuleCollider::Hit()
{
	m_col = 0xff0000;
}

CapsuleInfo CapsuleCollider::GetCapsuleInfo() const
{
	return m_info;
}

ColliderType CapsuleCollider::GetType() const
{
	return ColliderType::capsule;
}
