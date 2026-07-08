#pragma once
#include"Engine/Math/Vector2.h"

class IControllable
{
public:
	virtual ~IControllable() = default;

	virtual void Move(const Vector2& input) abstract;
	virtual void Jump()abstract;

	void Controll(bool is) {m_isControll = is;};

protected:
	bool m_isControll = false;
};

