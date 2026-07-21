#include "Ray.h"
#include"Engine/Core/Precompiled.h"
Ray::Ray(const Vector3& pos, const Vector3& dir, float length,const Vector3& offset)
{
	Vector3 position = pos;

	m_info.offset = offset;
	m_info.start = position + offset;
	m_info.direction = dir.Normalized();
	m_info.length = length;

	Vector3 vec = dir * length;

	m_info.end = { m_info.start.x + vec.x,m_info.start.y + vec.y ,m_info.start.z + vec.z };
}

void Ray::Update(const Vector3& pos)
{
	Vector3 position = pos;

	m_info.start = position + m_info.offset;

	Vector3 vec = m_info.direction * m_info.length;

	m_info.end = { m_info.start.x + vec.x,m_info.start.y + vec.y ,m_info.start.z + vec.z };
}

void Ray::Draw()
{
	DrawLine3D(m_info.start,m_info.end,0x00ff00);
}

RayInfo Ray::GetRayInfo() const
{
	return m_info;
}

HitInfo Ray::CheckModelCollision(int modelHandle)
{
	//DxLibの関数でモデルとの当たり判定をする
	auto result = MV1CollCheck_Line(modelHandle,-1,m_info.start,m_info.end);

	HitInfo info;
	info.isHit = result.HitFlag;
	info.hitPosition = result.HitPosition;

	return info;
}
