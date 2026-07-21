#pragma once
#include"Engine/Math/Vector3.h"

/// <summary>
/// コライダーの種類
/// </summary>
enum ColliderType
{
	capsule,
	sphere,
};

/// <summary>
/// 壁に当たった時の情報
/// </summary>
struct WallHitInfo
{
	bool isHit = false;
	Vector3 normal = {};
	Vector3 hitPos = {};
};

/// <summary>
/// コライダー基底クラス
/// </summary>
class Collider
{
public:
	virtual ~Collider() = default;

	virtual void Update(const Vector3& pos)abstract;

	virtual void Draw()abstract;
	/// <summary>
	/// モデル(壁)との当たり判定
	/// </summary>
	/// <param name="stageModelHandle">ステージモデル</param>
	/// <returns></returns>
	virtual std::vector<WallHitInfo> CheckWallCollision(int stageModelHandle)abstract;
	/// <summary>
	/// コライダーの種類を返す
	/// </summary>
	/// <returns></returns>
	virtual ColliderType GetType()const = 0;
	/// <summary>
	/// 半径を取得する
	/// </summary>
	/// <returns></returns>
	virtual float GetRadius()const = 0;

	bool IsActive() { return m_isActive; }

	void SetIsActive(bool is) { m_isActive = is; }

protected:
	bool m_isActive = true;
};

