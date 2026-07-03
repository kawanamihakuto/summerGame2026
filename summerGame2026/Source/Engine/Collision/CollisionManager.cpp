#include "CollisionManager.h"

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
	for(int i = 0;i < m_objects.size();++i)
	{
		for(int j = i + 1;j < m_objects.size();++j)
		{
			if(CanCollide(*m_objects[i], *m_objects[j]))
			{
				
			}
		}
	}
}

bool CollisionManager::CanCollide(ICollider& obj1, ICollider& obj2)
{
	if((obj1.GetCollisionLayer() & obj2.GetCollisionMask()) == 0)
	{
		return false;
	}
	if((obj2.GetCollisionLayer() & obj1.GetCollisionMask()) == 0)
	{
		return false;
	}

	return true;
}

MV1_COLL_RESULT_POLY_DIM CollisionManager::CheckCollCapsuleAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end, float radius)
{
	return MV1CollCheck_Capsule(handle, frameIndex,
		start, end, radius);
}

MV1_COLL_RESULT_POLY CollisionManager::CheckCollRayAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end)
{
	return MV1CollCheck_Line(handle,frameIndex,
		start, end);
}
