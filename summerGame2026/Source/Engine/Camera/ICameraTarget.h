#pragma once
#include"Engine/Math/Transform.h"

/// <summary>
/// カメラに渡したい情報
/// </summary>
struct CameraAnchor
{
	//カメラのターゲット
	Transform transform = {};
};

/// <summary>
/// カメラのターゲットになるオブジェクトの抽象基底クラス
/// </summary>
class ICameraTarget
{
public:
	virtual ~ICameraTarget() = default;

	virtual CameraAnchor GetCameraAnchor() const = 0;
};

