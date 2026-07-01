#pragma once
#include"Engine/Math/Vector3.h"

struct RayInfo
{
	Vector3 start;
	Vector3 end;
	Vector3 direction;
	float length;
};

class Ray
{
public:
	Ray();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="dir">伸ばす方向</param>
	/// <param name="length">長さ</param>
	void Init(const Vector3& pos, const Vector3& dir,float length);
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

private:
	RayInfo m_info;
};

