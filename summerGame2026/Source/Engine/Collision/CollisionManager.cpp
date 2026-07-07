#include "CollisionManager.h"
#include"CapsuleCollider.h"
#include"SphereCollider.h"

CollisionManager::CollisionManager()
{
}

void CollisionManager::AddObject(ICollider& object)
{
	m_objects.push_back(&object);
}

void CollisionManager::ClearObjects()
{
	m_objects.clear();
}

void CollisionManager::CheckAllCollisions()
{
	for (int i = 0; i < m_objects.size(); ++i)
	{
		for (int j = i + 1; j < m_objects.size(); ++j)
		{
			if (CanCollide(*m_objects[i], *m_objects[j]))
			{
				if (m_objects[i]->GetCollider().GetType() == ColliderType::capsule &&
					m_objects[j]->GetCollider().GetType() == ColliderType::capsule)
				{
					if (
						CheckCapsuleCapsule(
						static_cast<const CapsuleCollider&>(m_objects[i]->GetCollider()),
						static_cast<const CapsuleCollider&>(m_objects[j]->GetCollider())
						)) 
					{
						m_objects[i]->OnCollision(*m_objects[j]);
						m_objects[j]->OnCollision(*m_objects[i]);
					}
				}
				else if (m_objects[i]->GetCollider().GetType() == ColliderType::capsule &&
					m_objects[j]->GetCollider().GetType() == ColliderType::sphere)
				{
					if (CheckCapsuleSphere(
						static_cast<const CapsuleCollider&>(m_objects[i]->GetCollider()),
						static_cast<const SphereCollider&>(m_objects[j]->GetCollider())
						))
					{
						m_objects[i]->OnCollision(*m_objects[j]);
						m_objects[j]->OnCollision(*m_objects[i]);
					}
				}
				else if (m_objects[i]->GetCollider().GetType() == ColliderType::sphere &&
					m_objects[j]->GetCollider().GetType() == ColliderType::capsule)
				{
					if (CheckCapsuleSphere(
						static_cast<const CapsuleCollider&>(m_objects[j]->GetCollider()),
						static_cast<const SphereCollider&>(m_objects[i]->GetCollider())
						))
					{
						m_objects[i]->OnCollision(*m_objects[j]);
						m_objects[j]->OnCollision(*m_objects[i]);
					}
				}
				else if (m_objects[i]->GetCollider().GetType() == ColliderType::sphere &&
					m_objects[j]->GetCollider().GetType() == ColliderType::sphere)
				{
					if (CheckSphereSphere(
						static_cast<const SphereCollider&>(m_objects[i]->GetCollider()),
						static_cast<const SphereCollider&>(m_objects[j]->GetCollider())
						))
					{
						m_objects[i]->OnCollision(*m_objects[j]);
						m_objects[j]->OnCollision(*m_objects[i]);
					}
				}
			}
		}
	}
}

bool CollisionManager::CanCollide(ICollider& obj1, ICollider& obj2)
{
	if ((obj1.GetCollisionLayer() & obj2.GetCollisionMask()) == 0)
	{
		return false;
	}
	if ((obj2.GetCollisionLayer() & obj1.GetCollisionMask()) == 0)
	{
		return false;
	}

	return true;
}

bool CollisionManager::CheckSphereSphere(const SphereCollider& a, const SphereCollider& b)
{
	SphereInfo infoA = a.GetSphereInfo();
	SphereInfo infoB = b.GetSphereInfo();

	Vector3 vec = infoA.pos - infoB.pos;

	if (vec.Length() < (infoA.radius + infoB.radius))
	{
		return true;
	}

	return false;
}

bool CollisionManager::CheckCapsuleCapsule(const CapsuleCollider& a, const CapsuleCollider& b)
{
	return false;
}

bool CollisionManager::CheckCapsuleSphere(const CapsuleCollider& a, const SphereCollider& b)
{
	CapsuleInfo capsule = a.GetCapsuleInfo();
	SphereInfo sphere = b.GetSphereInfo();

	Vector3 capLineVec = capsule.end - capsule.start;
	Vector3 sphereCapvec = sphere.pos - capsule.start;

	float t = sphereCapvec.Dot(capLineVec) / capLineVec.Dot(capLineVec);

	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 pos = capsule.start + capLineVec * t;

	Vector3 vec = sphere.pos - pos;

#ifdef _DEBUG
	DrawLine3D(sphere.pos,pos,0xff00ff);

	DrawFormatString(16, 420, 0xffffff,
		L"start:%f,%f,%f",
		capsule.start.x,
		capsule.start.y,
		capsule.start.z);

	DrawFormatString(16, 438, 0xffffff,
		L"end:%f,%f,%f",
		capsule.end.x,
		capsule.end.y,
		capsule.end.z);

	DrawFormatString(16, 400, 0xffffff, L"t:%f", t);
	DrawFormatString(16,364,0xffffff,L"pos:%f,%f,%f",pos.x,pos.y,pos.z);
	DrawFormatString(16,382,0xffffff,L"vecLen:%f",vec.Length());
#endif // _DEBUG

	if (vec.Length() < capsule.radius + sphere.radius)
	{
		return true;
	}

	return false;
}

MV1_COLL_RESULT_POLY_DIM CollisionManager::CheckCollCapsuleAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end, float radius)
{
	return MV1CollCheck_Capsule(handle, frameIndex,
		start, end, radius);
}

MV1_COLL_RESULT_POLY CollisionManager::CheckCollRayAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end)
{
	return MV1CollCheck_Line(handle, frameIndex,
		start, end);
}
