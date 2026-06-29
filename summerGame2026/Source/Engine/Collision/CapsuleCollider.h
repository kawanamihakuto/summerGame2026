#pragma once
#include "Collider.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Math/Transform.h"

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	void Init(const Transform& transform,float radius,float height);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	void Update(const Transform& transform);
	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Draw();
	/// <summary>
	/// コライダーの種類を返す
	/// </summary>
	/// <returns></returns>
	ColliderType GetType()const override;

private:
	float m_radius;
	float m_height;

	Vector3 m_start;
	Vector3 m_end;
};

