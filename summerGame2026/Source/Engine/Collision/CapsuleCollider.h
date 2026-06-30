#pragma once
#include "Collider.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Math/Transform.h"

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
	CapsuleCollider();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	void Init(const Vector3& pos,float radius,float height);
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

