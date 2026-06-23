#pragma once
#include"Engine/Core/GameObject.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

private:
	int m_modelHandle;
};

