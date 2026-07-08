#pragma once
#include"Engine/Math/Vector3.h"

struct RayInfo
{
	Vector3 start;
	Vector3 end;
	Vector3 direction;
	float length;
	Vector3 offset;
};

struct HitInfo
{
	bool isHit = false;
	Vector3 hitPosition = {};
};

class Ray
{
public:
	Ray(const Vector3& pos, const Vector3& dir, float length, const Vector3& offset);
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

	HitInfo CheckModelCollision(int modelHandle);

private:
	RayInfo m_info;
};

