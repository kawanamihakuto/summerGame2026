#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"

struct TransForm
{
	//ポジション
	Vector3 position{ 0.0f,0.0f,0.0f };
	//回転
	Vector3 rotation{ 0.0f,0.0f,0.0f };
	//拡大縮小
	Vector3 scale{ 1.0f,1.0f,1.0f };

	/// <summary>
	/// ワールド行列に変換
	/// </summary>
	/// <returns>ワールド行列</returns>
	Matrix4x4 GetWorldMatrix()const;
	/// <summary>
	/// オブジェクトの前方向ベクトルを返す
	/// </summary>
	/// <returns>前方向ベクトル</returns>
	Vector3 Forward()const;
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="move">移動ベクトル</param>
	void Translate(const Vector3& move);
	/// <summary>
	/// 拡大縮小をセット
	/// </summary>
	/// <param name="scale">拡大縮小倍率</param>
	void SetScale(const Vector3& s);
};
