#pragma once
#include"Engine/Collision/CollisionLayer.h"
#include"Engine/Collision/Collider.h"
#include"Engine/Collision/Ray.h"

class ICollider
{
public:
	virtual ~ICollider() = default;

	virtual const Collider& GetCollider()const abstract;
	virtual const Ray& GetRay()const abstract;

	/// <summary>
	/// 自身のコリジョンレイヤーを返す
	/// </summary>
	/// <returns></returns>
	virtual CollisionLayer GetCollisionLayer()const abstract;
	/// <summary>
	/// 当たり判定をするコリジョンレイヤーを返す
	/// </summary>
	/// <returns></returns>
	virtual CollisionLayer GetCollisionMask()const abstract;
	/// <summary>
	/// 当たった
	/// </summary>
	/// <param name="other"></param>
	virtual void OnCollision(ICollider& other) abstract;
};

