#pragma once
#include"Engine/Core/Precompiled.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Core/ICollider.h"

class ICollider;
class CollisionManager
{
public:
	CollisionManager();
	/// <summary>
	/// オブジェクトの追加
	/// </summary>
	/// <param name="object"></param>
	void AddObject(ICollider& object);
	/// <summary>
	/// オブジェクトのクリア
	/// </summary>
	void ClearObjects();
	/// <summary>
	/// 衝突のチェック
	/// </summary>
	void CheckAllCollisions();

private:
	/// <summary>
	/// 2つのオブジェクトが衝突するかを判定
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	/// <returns></returns>
	bool CanCollide(ICollider& obj1, ICollider& obj2);
	//オブジェクトたち
	std::vector<ICollider*> m_objects;

public:
	static MV1_COLL_RESULT_POLY_DIM CheckCollCapsuleAndPolygon(int handle,int frameIndex,const Vector3& start,const Vector3& end,float radius);

	static MV1_COLL_RESULT_POLY CheckCollRayAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end);
};

