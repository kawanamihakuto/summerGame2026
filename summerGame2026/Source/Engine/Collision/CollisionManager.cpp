#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

void CollisionManager::AddObject(GameObject& object)
{
	m_objects.push_back(std::make_shared<GameObject>(object));
}

void CollisionManager::ClearObjects()
{
	m_objects.clear();
}

void CollisionManager::CheckCollisions()
{
}

bool CollisionManager::CanCollide(GameObject& obj1, GameObject& obj2)
{
	return false;
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
