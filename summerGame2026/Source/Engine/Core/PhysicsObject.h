#pragma once
#include "GameObject.h"
#include"Precompiled.h"
#include"Engine/Collision/Collider.h"
#include"Engine/Collision/Ray.h"

class PhysicsObject : public GameObject
{
public:
	/// <summary>
	/// 壁との当たり判定
	/// </summary>
	/// <param name="stageModelHandle"></param>
	void ResolveWallVelocity(int stageModelHandle);
	/// <summary>
	/// 地面との当たり判定
	/// </summary>
	/// <param name="stageModelHandle"></param>
	void GroundCollision(int stageModelHandle);
	/// <summary>
	/// 重力
	/// </summary>
	void Gravity();

	virtual void Init()override abstract;
	virtual void End()override abstract;
	virtual void Update()override abstract;
	virtual void Draw()override abstract;

	/// <summary>
	/// 速度を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetVelocity() const { return m_velocity; }

	/// <summary>
	/// 地面にいるかどうか
	/// </summary>
	/// <returns></returns>
	bool IsGround() { return m_isGround; }

protected:
	//速度
	Vector3 m_velocity = {};
	//壁との当たり判定用コライダー
	std::unique_ptr<Collider> m_stageCollider;
	//キャラクターとの当たり判定用コライダー
	std::unique_ptr<Collider> m_bodyCollider;
	//地面判定用レイ
	std::vector<std::unique_ptr<Ray>> m_ray;
	//地面にいるかどうか
	bool m_isGround = false;
};

