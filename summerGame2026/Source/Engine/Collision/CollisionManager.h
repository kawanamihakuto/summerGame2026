#pragma once
#include"Engine/Core/Precompiled.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Collision/ICollider.h"

class SphereCollider;
class CapsuleCollider;
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

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	bool CheckSphereSphere(const SphereCollider& a, const SphereCollider& b);
	/// <summary>
	/// カプセルとカプセルの当たり判定
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	bool CheckCapsuleCapsule(const CapsuleCollider& a, const CapsuleCollider& b);
	/// <summary>
	/// カプセルと球の当たり判定
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	bool CheckCapsuleSphere(const CapsuleCollider& a, const SphereCollider& b);

	//オブジェクトたち
	std::vector<ICollider*> m_objects;
};

