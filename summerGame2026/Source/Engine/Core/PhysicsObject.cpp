#include "PhysicsObject.h"

namespace
{
	//重力
	constexpr float kGravity = 0.9f;
}

void PhysicsObject::ResolveWallVelocity(int stageModelHandle)
{
	//DxLibの関数でモデルとの当たり判定をする
	auto hits = m_stageCollider->CheckWallCollision(stageModelHandle);

	for (const auto& hit : hits)
	{
		Vector3 vel = { m_velocity.x,0.0f,m_velocity.z };

		float dot = vel.Dot(hit.normal);

		if (dot < 0.0f)
		{
			//速度を修正
			m_velocity -= hit.normal * dot;
		}
	}
}

void PhysicsObject::GroundCollision(int stageModelHandle)
{
	float groundHeight = 10000.0f;
	m_isGround = false;
	Vector3 result = {};
	//上がってるとき
	if (m_velocity.y > 0.0f)
	{
		groundHeight = 10000.0f;

		//全部のレイ
		for (auto& ray : m_ray)
		{
			//チェック
			auto info = ray->CheckModelCollision(stageModelHandle);
			//当たってたら
			if (info.isHit)
			{
				//一番低いのを保存
				if (groundHeight > info.hitPosition.y)
				{
					groundHeight = info.hitPosition.y;
				}

				if (groundHeight < 10000.0f)
				{
					//修正後の位置をだす
					Vector3 offset = ray->GetRayInfo().offset;
					result = { info.hitPosition.x - offset.x,groundHeight - (ray->GetRayInfo().start.y - ray->GetRayInfo().end.y),info.hitPosition.z - offset.z};
					m_velocity.y = 0.0f;
				}
			}
		}
		
		if (groundHeight != 10000.0f)
		{
			//トランスフォームに適用
			m_transform.SetPosition(result);
		}
	}
	else//下がってるとき
	{
		groundHeight = -10000.0f;
		//全部のレイ
		for (auto& ray : m_ray)
		{
			//チェック
			auto info = ray->CheckModelCollision(stageModelHandle);

			//当たってたら
			if (info.isHit)
			{
				//一番高いのを保存
				if (groundHeight < info.hitPosition.y)
				{
					groundHeight = info.hitPosition.y;
				}

				if (groundHeight > -10000.0f)
				{
					//修正後の位置をだす
					Vector3 offset = ray->GetRayInfo().offset;
					result = { info.hitPosition.x - offset.x,groundHeight,info.hitPosition.z - offset.z };
					m_velocity.y = 0.0f;
					//地面にいるよ
					m_isGround = true;
				}
			}
		}

		if (m_isGround)
		{
			//トランスフォームに適用
			m_transform.SetPosition(result);
		}
	}
}

void PhysicsObject::Gravity()
{
	//重力だよ
	m_velocity.y -= kGravity;
}
