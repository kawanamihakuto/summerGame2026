#include "CapsuleCollider.h"
#include"Engine/Core/Precompiled.h"

CapsuleCollider::CapsuleCollider(const Vector3& pos, float radius, float height):
	m_info({}, {},0.0f,0.0f),
	m_col(0x00ff00)
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
	if (m_isActive)
	{
		DrawCapsule3D(m_info.start, m_info.end,
			m_info.radius, 8, m_col, m_col, false);
	}
}

std::vector<WallHitInfo> CapsuleCollider::CheckWallCollision(int stageModelHandle)
{
	auto result = MV1CollCheck_Capsule(stageModelHandle, -1,
		m_info.start, m_info.end, m_info.radius);
	
	std::vector<WallHitInfo> info;
	if (result.HitNum > 0)
	{
		info.resize(result.HitNum);
		for (int i = 0; i < result.HitNum; i++)
		{
			auto& poly = result.Dim[i];

			Vector3 normal = { poly.Normal.x,poly.Normal.y,poly.Normal.z };
			normal.Normalize();

			info[i].isHit = true;
			info[i].normal = normal;
		}
	}

	MV1CollResultPolyDimTerminate(result);

	return info;
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
