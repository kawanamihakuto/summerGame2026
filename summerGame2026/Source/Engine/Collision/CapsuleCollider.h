#pragma once
#include "Collider.h"
#include"Engine/Math/Vector3.h"

struct CapsuleInfo
{
	Vector3 start;
	Vector3 end;
	float radius;
	float height;
};

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(const Vector3& pos, float radius, float height);

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

	std::vector<WallHitInfo> CheckWallCollision(int stageModelHandle)override;

	/// <summary>
	/// 当たり判定デバッグ用
	/// </summary>
	void Hit();

	/// <summary>
	/// カプセルの情報を取得する
	/// </summary>
	/// <returns></returns>
	CapsuleInfo GetCapsuleInfo()const;

	/// <summary>
	/// コライダーの種類を返す
	/// </summary>
	/// <returns></returns>
	ColliderType GetType()const override;

private:
	CapsuleInfo m_info;

	int m_col;
};

