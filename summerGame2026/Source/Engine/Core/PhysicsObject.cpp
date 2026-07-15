#include "PhysicsObject.h"

namespace
{
	constexpr float kGravity = 0.9f;
}

void PhysicsObject::ResolveWallVelocity(int stageModelHandle)
{
	auto hits = m_collider->CheckWallCollision(stageModelHandle);

	for (const auto& hit : hits)
	{
		Vector3 vel = { m_velocity.x,0.0f,m_velocity.z };

		float dot = vel.Dot(hit.normal);

		if (dot < 0.0f)
		{
			m_velocity -= hit.normal * dot;
		}
	}
}

void PhysicsObject::GroundCollision(int stageModelHandle)
{
	float groundHeight = -10000.0f;
	m_isGround = false;

	Vector3 result = {};
	for (auto& ray : m_ray)
	{
		auto info = ray->CheckModelCollision(stageModelHandle);

		if (info.isHit)
		{
			if (groundHeight < info.hitPosition.y)
			{
				groundHeight = info.hitPosition.y;
			}

			if (groundHeight > -10000.0f)
			{
				Vector3 offset = ray->GetRayInfo().offset;
				result = { info.hitPosition.x - offset.x,groundHeight,info.hitPosition.z - offset.z };
				m_isGround = true;
				m_velocity.y = 0.0f;
			}
		}
	}

	if (m_isGround)
	{
		m_transform.SetPosition(result);
	}
}

void PhysicsObject::Gravity()
{
	m_velocity.y -= kGravity;
}
