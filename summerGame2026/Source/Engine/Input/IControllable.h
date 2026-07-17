#pragma once
#include"Engine/Math/Vector2.h"

class IControllable
{
public:
	virtual ~IControllable() = default;

	virtual void Move(const Vector2& input) abstract;
	virtual void Jump()abstract;

	virtual void Controll()abstract;

	virtual void ExitControll()abstract;
protected:
};

