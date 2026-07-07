#pragma once
#include "Collider.h"
#include"Engine/Math/Vector3.h"

struct SphereInfo
{
	Vector3 pos;
	float radius;
};

class SphereCollider : public Collider
{
public:
	SphereCollider();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="radius">半径</param>
	void Init(const Vector3& pos, float radius);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">ポジション</param>
	void Update(const Vector3& pos);
	/// <summary>
	/// デバッグ表示用更新
	/// </summary>
	/// <param name="pos"></param>
	void ReUpdate(const Vector3& pos);
	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Draw();
	/// <summary>
	/// 当たり判定デバッグ用
	/// </summary>
	void Hit();
	/// <summary>
	/// 球の情報を取得する
	/// </summary>
	/// <returns></returns>
	SphereInfo GetSphereInfo()const;

	ColliderType GetType()const override;

private:
	SphereInfo m_info;
};

