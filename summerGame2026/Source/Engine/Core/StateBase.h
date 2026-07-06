#pragma once
class StateBase
{
public:
	virtual ~StateBase() = default;

	virtual void Enter() = 0;

	virtual void Update() = 0;

	virtual void Exit() = 0;
};

