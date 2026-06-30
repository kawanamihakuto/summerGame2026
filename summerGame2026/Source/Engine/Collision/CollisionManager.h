#pragma once
#include"Engine/Core/Precompiled.h"
#include"Engine/Math/Vector3.h"
class CollisionManager
{
public:
	static MV1_COLL_RESULT_POLY_DIM CheckCollCapsuleAndPolygon(int handle,int frameIndex,const Vector3& start,const Vector3& end,float radius);
};

