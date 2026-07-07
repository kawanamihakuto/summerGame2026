#pragma once
#include"Engine/Math/Vector3.h"

class ITarget
{
public:
	virtual~ITarget() = default;

	virtual Vector3 GetPosition()const abstract;
};

