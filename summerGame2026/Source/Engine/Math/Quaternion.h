#pragma once
#include"Vector3.h"

class Quaternion
{
public:
	float x, y, z, w;
	/// <summary>
	/// コンストラクタで単位クォータニオン(回転なし)を生成
	/// </summary>
	Quaternion();
	/// <summary>
	/// 各成分を指定して生成
	/// </summary>
	Quaternion(float x, float y, float z, float w);

	/// <summary>
	/// 回転なしのクォータニオンを取得
	/// </summary>
	/// <returns></returns>
	static Quaternion Identity();

	/// <summary>
	/// クォータニオンの長さを取得
	/// </summary>
	/// <returns></returns>
	float Length()const;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns></returns>
	Quaternion Normalize()const;

	/// <summary>
	/// 共役クォータニオンを取得
	/// </summary>
	/// <returns></returns>
	Quaternion Conjugate()const;

	/// <summary>
	/// 逆クォータニオンを取得
	/// </summary>
	/// <returns></returns>
	Quaternion Inverse()const;

	/// <summary>
	/// ベクトルへ回転を適用 
	/// </summary>
	/// <param name="vec">ベクトル</param>
	/// <returns></returns>
	Vector3 Rotate(const Vector3& vec)const;

	Quaternion operator*(const Quaternion& q)const;
}

