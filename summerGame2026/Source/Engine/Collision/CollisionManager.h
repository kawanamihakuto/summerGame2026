#pragma once
#include"Engine/Core/Precompiled.h"
#include"Engine/Math/Vector3.h"

class GameObject;
class CollisionManager
{
public:
	CollisionManager();
	/// <summary>
	/// オブジェクトの追加
	/// </summary>
	/// <param name="object"></param>
	void AddObject(GameObject& object);
	/// <summary>
	/// オブジェクトのクリア
	/// </summary>
	void ClearObjects();
	/// <summary>
	/// 衝突のチェック
	/// </summary>
	void CheckCollisions();

private:
	/// <summary>
	/// 2つのオブジェクトが衝突するかを判定
	/// </summary>
	/// <param name="obj1"></param>
	/// <param name="obj2"></param>
	/// <returns></returns>
	bool CanCollide(GameObject& obj1, GameObject& obj2);
	//オブジェクトたち
	std::vector<std::shared_ptr<GameObject>> m_objects;

public:
	static MV1_COLL_RESULT_POLY_DIM CheckCollCapsuleAndPolygon(int handle,int frameIndex,const Vector3& start,const Vector3& end,float radius);

	static MV1_COLL_RESULT_POLY CheckCollRayAndPolygon(int handle, int frameIndex, const Vector3& start, const Vector3& end);
};

