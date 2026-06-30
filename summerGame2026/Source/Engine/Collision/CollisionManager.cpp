#include "CollisionManager.h"

MV1_COLL_RESULT_POLY_DIM CollisionManager::CheckCollCapsuleAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end, float radius)
{
	return MV1CollCheck_Capsule(handle, frameIndex,
		start.ChangeDxVector(), end.ChangeDxVector(), radius);
}

MV1_COLL_RESULT_POLY CollisionManager::CheckCollRayAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end)
{
	return MV1CollCheck_Line(handle,frameIndex,
		start.ChangeDxVector(),end.ChangeDxVector());
}
