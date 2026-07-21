#pragma once
#include"Engine/Math/Vector2.h"

/// <summary>
/// 操作することができるオブジェクトの抽象基底クラス
/// </summary>
class IControllable
{
public:
	virtual ~IControllable() = default;
	//移動
	virtual void Move(const Vector2& input) abstract;
	//ジャンプ
	virtual void Jump()abstract;
	//操作対象になったときに呼ばれる
	virtual void Controll()abstract;
	//操作対象から外れたときに呼ばれる
	virtual void ExitControll()abstract;
protected:
};

