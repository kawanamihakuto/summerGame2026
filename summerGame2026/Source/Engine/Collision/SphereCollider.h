#pragma once
#include "Collider.h"
#include"Engine/Math/Vector3.h"

/// <summary>
/// 球の情報
/// </summary>
struct SphereInfo
{
	Vector3 pos;
	float radius;
};

/// <summary>
/// 球のコライダー
/// </summary>
class SphereCollider : public Collider
{
public:
	SphereCollider(const Vector3& pos, float radius);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">ポジション</param>
	void Update(const Vector3& pos)override;
	/// <summary>
	/// デバッグ表示用更新
	/// </summary>
	/// <param name="pos"></param>
	void ReUpdate(const Vector3& pos);
	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Draw()override;

	/// <summary>
	/// モデル(壁)との当たり判定
	/// </summary>
	std::vector<WallHitInfo> CheckWallCollision(int stageModelHandle)override;
	/// <summary>
	/// 球の情報を取得する
	/// </summary>
	/// <returns></returns>
	SphereInfo GetSphereInfo()const;
	/// <summary>
	/// コライダータイプのゲッター
	/// </summary>
	/// <returns></returns>
	ColliderType GetType()const override;
	/// <summary>
	/// 半径のゲッター
	/// </summary>
	/// <returns></returns>
	float GetRadius()const override;

private:
	SphereInfo m_info;
};

