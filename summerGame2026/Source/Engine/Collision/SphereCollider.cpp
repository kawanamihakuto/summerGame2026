#include "SphereCollider.h"

SphereCollider::SphereCollider(const Vector3& pos, float radius):
	m_info({}, 0.0f)
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
	if (m_isActive)
	{
		DrawSphere3D(m_info.pos, m_info.radius, 8, 0x00ff00, 0x00ff00, false);
	}
}

std::vector<WallHitInfo> SphereCollider::CheckWallCollision(int stageModelHandle)
{
	//DxLibの関数でモデルとの当たり判定をする
	auto result = MV1CollCheck_Sphere(stageModelHandle, -1,
		m_info.pos, m_info.radius);

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
			info[i] .normal = normal;
			info[i].hitPos = poly.HitPosition;
		}
	}

	MV1CollResultPolyDimTerminate(result);

	return info;
}
SphereInfo SphereCollider::GetSphereInfo() const
{
	return m_info;
}

ColliderType SphereCollider::GetType() const
{
	return ColliderType::sphere;
}

float SphereCollider::GetRadius() const
{
	return m_info.radius;
}
