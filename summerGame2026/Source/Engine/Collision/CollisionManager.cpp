#include "CollisionManager.h"
#include"CapsuleCollider.h"
#include"SphereCollider.h"
#include"ICollider.h"

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
					auto res = CheckCapsuleCapsule(
						static_cast<const CapsuleCollider&>(m_objects[i]->GetCollider()),
						static_cast<const CapsuleCollider&>(m_objects[j]->GetCollider())
					);

					auto res2 = res;
					res2.normal *= 1.0f;

					if (res.isHit)
					{
						m_objects[i]->OnCollision(*m_objects[j],res);
						m_objects[j]->OnCollision(*m_objects[i],res2);
					}
				}
				else if (m_objects[i]->GetCollider().GetType() == ColliderType::capsule &&
					m_objects[j]->GetCollider().GetType() == ColliderType::sphere)
				{
					auto res = CheckCapsuleSphere(
						static_cast<const CapsuleCollider&>(m_objects[i]->GetCollider()),
						static_cast<const SphereCollider&>(m_objects[j]->GetCollider())
					);

					auto res2 = res;
					res2.normal *= 1.0f;

					if (res.isHit)
					{
						m_objects[i]->OnCollision(*m_objects[j], res);
						m_objects[j]->OnCollision(*m_objects[i], res2);
					}
				}
				else if (m_objects[i]->GetCollider().GetType() == ColliderType::sphere &&
					m_objects[j]->GetCollider().GetType() == ColliderType::capsule)
				{
					auto res = CheckCapsuleSphere(
						static_cast<const CapsuleCollider&>(m_objects[j]->GetCollider()),
						static_cast<const SphereCollider&>(m_objects[i]->GetCollider())
					);

					auto res2 = res;
					res2.normal *= 1.0f;

					if (res.isHit)
					{
						m_objects[i]->OnCollision(*m_objects[j], res);
						m_objects[j]->OnCollision(*m_objects[i], res2);
					}
				}
				else if (m_objects[i]->GetCollider().GetType() == ColliderType::sphere &&
					m_objects[j]->GetCollider().GetType() == ColliderType::sphere)
				{
					auto res = CheckSphereSphere(
						static_cast<const SphereCollider&>(m_objects[i]->GetCollider()),
						static_cast<const SphereCollider&>(m_objects[j]->GetCollider())
					);

					auto res2 = res;
					res2.normal *= 1.0f;

					if (res.isHit)
					{
						m_objects[i]->OnCollision(*m_objects[j], res);
						m_objects[j]->OnCollision(*m_objects[i], res2);
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

CollisionResult CollisionManager::CheckSphereSphere(const SphereCollider& a, const SphereCollider& b)
{
	SphereInfo infoA = a.GetSphereInfo();
	SphereInfo infoB = b.GetSphereInfo();

	Vector3 vec = infoA.pos - infoB.pos;

	float radiusSum = infoA.radius + infoB.radius;

	if (vec.Length() < radiusSum)
	{

		Vector3 normal = vec.Normalized();
				
		if(vec.Length() <= 0.01f)
		{
			normal = { 1.0f,0.0f,0.0f };
		}

		float penetartion = radiusSum - vec.Length();

		CollisionResult result = { true,normal,penetartion };

		return result;
	}

	return CollisionResult();
}

CollisionResult CollisionManager::CheckCapsuleCapsule(const CapsuleCollider& a, const CapsuleCollider& b)
{
	return CollisionResult();
}

CollisionResult CollisionManager::CheckCapsuleSphere(const CapsuleCollider& a, const SphereCollider& b)
{
	CapsuleInfo capsule = a.GetCapsuleInfo();
	SphereInfo sphere = b.GetSphereInfo();

	Vector3 capLineVec = capsule.end - capsule.start;
	Vector3 sphereCapvec = sphere.pos - capsule.start;

	float t = sphereCapvec.Dot(capLineVec) / capLineVec.Dot(capLineVec);

	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 pos = capsule.start + capLineVec * t;

	Vector3 vec = sphere.pos - pos;

	float radiusSum = capsule.radius + sphere.radius;

	if (vec.Length() < radiusSum)
	{
		Vector3 normal = vec.Normalized();

		float penetartion = radiusSum - vec.Length();

		CollisionResult result = { true,normal,penetartion };

		return result;
	}

	return CollisionResult();
}
