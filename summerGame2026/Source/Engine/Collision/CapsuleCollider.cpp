#include "CapsuleCollider.h"
#include"Engine/Core/Precompiled.h"

CapsuleCollider::CapsuleCollider(const Vector3& pos, float radius, float height):
	m_info({}, {},0.0f,0.0f)
{
	//各情報を設定
	Vector3 position = pos;
	m_info.radius = radius;
	m_info.height = height;
	m_info.start = position + Vector3(0.0f, m_info.radius, 0.0f);
	m_info.end = position + Vector3(0.0f, m_info.height - m_info.radius, 0.0f);
}

void CapsuleCollider::Update(const Vector3& pos)
{
	//位置を更新
	Vector3 position = pos;
	m_info.start = position + Vector3(0.0f, m_info.radius, 0.0f);
	m_info.end = position + Vector3(0.0f, m_info.height - m_info.radius, 0.0f);
}

void CapsuleCollider::Draw()
{
	//描画
	if (m_isActive)
	{
		DrawCapsule3D(m_info.start, m_info.end,
			m_info.radius, 8, 0xff0000,0xff0000, false);
	}
}

std::vector<WallHitInfo> CapsuleCollider::CheckWallCollision(int stageModelHandle)
{
	//DxLibの関数でモデルとの当たり判定をする
	auto result = MV1CollCheck_Capsule(stageModelHandle, -1,
		m_info.start, m_info.end, m_info.radius);
	
	std::vector<WallHitInfo> info;
	if (result.HitNum > 0)
	{
		//当たった情報をポリゴン分保存
		info.resize(result.HitNum);
		for (int i = 0; i < result.HitNum; i++)
		{
			auto& poly = result.Dim[i];

			Vector3 normal = { poly.Normal.x,poly.Normal.y,poly.Normal.z };
			normal.Normalize();

			info[i].isHit = true;
			info[i].normal = normal;
			info[i].hitPos = poly.HitPosition;
		}
	}

	MV1CollResultPolyDimTerminate(result);

	return info;
}

CapsuleInfo CapsuleCollider::GetCapsuleInfo() const
{
	return m_info;
}

ColliderType CapsuleCollider::GetType() const
{
	return ColliderType::capsule;
}

void CapsuleCollider::SetHeight(float height)
{
	m_info.height = height;
}

float CapsuleCollider::GetRadius() const
{
	return m_info.radius;
}
