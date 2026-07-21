#pragma once
#include"Engine/Core/Precompiled.h"
#include"Engine/Math/Vector3.h"

/// <summary>
/// コリジョン情報
/// </summary>
struct CollisionResult
{
	bool isHit = false;
	Vector3 normal = {};
	float penetration = 0.0f;
};

class SphereCollider;
class CapsuleCollider;
class ICollider;
/// <summary>
/// コライダーの当たり判定を行う
/// </summary>
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
	/// <summary>
	/// 特定のオブジェクトをクリアする
	/// </summary>
	void RemoveObject(ICollider* collider);

private:
	/// <summary>
	/// 2つのオブジェクトが衝突するかを判定
	/// </summary>
	bool CanCollide(ICollider& obj1, ICollider& obj2);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	CollisionResult CheckSphereSphere(const SphereCollider& a, const SphereCollider& b);
	/// <summary>
	/// カプセルとカプセルの当たり判定
	/// </summary>
	CollisionResult CheckCapsuleCapsule(const CapsuleCollider& a, const CapsuleCollider& b);
	/// <summary>
	/// カプセルと球の当たり判定
	/// </summary>
	CollisionResult CheckCapsuleSphere(const CapsuleCollider& a, const SphereCollider& b);

	//オブジェクトたち
	std::vector<ICollider*> m_objects;
};

