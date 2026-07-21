#pragma once
#include"Engine/Math/Vector3.h"

/// <summary>
/// 敵にターゲットされるオブジェクトの抽象基底クラス
/// </summary>
class ITarget
{
public:
	virtual~ITarget() = default;

	virtual Vector3 GetPosition()const abstract;
};

