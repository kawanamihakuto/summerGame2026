#pragma once
#include"Engine/Math/Vector3.h"

/// <summary>
/// レイの情報
/// </summary>
struct RayInfo
{
	Vector3 start;
	Vector3 end;
	Vector3 direction;
	float length;
	Vector3 offset;
};

/// <summary>
/// モデルとの当たり判定の情報
/// </summary>
struct HitInfo
{
	bool isHit = false;
	Vector3 hitPosition = {};
	Vector3 normal = {};
};

class Ray
{
public:
	Ray(const Vector3& pos, const Vector3& dir, float length, const Vector3& widthOffset,float heightOffset);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos">ポジション</param>
	void Update(const Vector3& pos);
	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Draw();
	/// <summary>
	/// Rayの情報を取得する
	/// </summary>
	/// <returns></returns>
	RayInfo GetRayInfo()const;
	/// <summary>
	/// モデルとの当たり判定
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <returns></returns>
	HitInfo CheckModelCollision(int modelHandle);
	/// <summary>
	/// 高さをセットする
	/// </summary>
	/// <param name="height"></param>
	void SetOffSetAndLength(const Vector3& widthOffset,float heightOffset,float length);

private:
	RayInfo m_info;
};

