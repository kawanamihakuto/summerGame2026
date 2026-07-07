#pragma once
#include"Engine/Math/Transform.h"

struct CameraAnchor
{
	Transform transform;
	float groundHeight;
};

class ICameraTarget
{
public:
	virtual ~ICameraTarget() = default;

	virtual CameraAnchor GetCameraAnchor() const = 0;
};

