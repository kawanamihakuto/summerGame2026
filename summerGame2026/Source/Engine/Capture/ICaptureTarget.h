#pragma once
#include"Engine/Input/IControllable.h"
#include"Engine/Camera/ICameraTarget.h"

class ICaptureTarget : public IControllable,public ICameraTarget
{
public:
	virtual ~ICaptureTarget() = default;

	virtual Matrix4x4 GetHatMatrix()const abstract;
};

