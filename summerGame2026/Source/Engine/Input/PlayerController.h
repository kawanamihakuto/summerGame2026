#pragma once
#include"IControllable.h"

class PlayerController
{
public:
	PlayerController();
	~PlayerController();

	void Update();

	void SetTarget(IControllable* target);

private:
	IControllable* m_target = nullptr;
};

