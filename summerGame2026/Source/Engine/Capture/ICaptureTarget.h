#pragma once
#include"Engine/Input/IControllable.h"
#include"Engine/Camera/ICameraTarget.h"

class ICaptureTarget : public IControllable,public ICameraTarget
{
public:
	virtual ~ICaptureTarget() = default;

	virtual Vector3 GetPosition()abstract;
	virtual int GetModel()abstract;
};

