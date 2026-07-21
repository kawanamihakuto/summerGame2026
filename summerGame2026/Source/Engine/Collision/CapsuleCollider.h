#pragma once
#include "Collider.h"
#include"Engine/Math/Vector3.h"

/// <summary>
/// カプセルの情報
/// </summary>
struct CapsuleInfo
{
	//開始位置
	Vector3 start;
	//終了位置
	Vector3 end;
	//半径
	float radius;
	//高さ
	float height;
};

/// <summary>
/// カプセルコライダー
/// </summary>
class CapsuleCollider : public Collider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	CapsuleCollider(const Vector3& pos, float radius, float height);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">ポジション</param>
	void Update(const Vector3& pos)override;
	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Draw()override;
	/// <summary>
	/// モデル(壁)との当たり判定
	/// </summary>
	/// <param name="stageModelHandle"></param>
	/// <returns></returns>
	std::vector<WallHitInfo> CheckWallCollision(int stageModelHandle)override;
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
	/// <summary>
	/// カプセルと高さを変更する
	/// </summary>
	/// <param name="height"></param>
	void SetHeight(float height);
	//半径取得
	float GetRadius()const override;

private:
	CapsuleInfo m_info;
};

