#pragma once
#include"IControllable.h"
#include"Game/Item/Hat.h"

class PlayerController
{
public:
	PlayerController(Hat& hat);
	~PlayerController();

	void Update();

	void SetTarget(IControllable* target);

private:
	IControllable* m_target = nullptr;
	Hat& m_hat;
};

