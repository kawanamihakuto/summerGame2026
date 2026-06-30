#include "Ray.h"

Ray::Ray():
	m_info({}, {}, {},0.0f)
{
}

void Ray::Init(const Vector3& pos, const Vector3& dir, float length)
{
	Vector3 position = pos;

	m_info.start = position;
	m_info.direction = dir.Normalized();
	m_info.length = length;

	Vector3 vec = dir * length;

	m_info.end = { position.x + vec.x,position.y + vec.y ,position.z + vec.z };
}

void Ray::Update(const Vector3& pos)
{
	Vector3 position = pos;

	m_info.start = position;

	Vector3 vec = m_info.direction * m_info.length;

	m_info.end = { position.x + vec.x,position.y + vec.y ,position.z + vec.z };
}

void Ray::Draw()
{
	DrawLine3D(m_info.start.ChangeDxVector(),m_info.end.ChangeDxVector(),0x00ff00);
}

RayInfo Ray::GetRayInfo() const
{
	return m_info;
}
