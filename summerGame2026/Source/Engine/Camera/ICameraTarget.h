#pragma once
#include"Engine/Math/Transform.h"

struct CameraAnchor
{
	Transform transform = {};
};

class ICameraTarget
{
public:
	virtual ~ICameraTarget() = default;

	virtual CameraAnchor GetCameraAnchor() const = 0;
};

