#pragma once
#include"Engine/Core/GameObject.h"

class AnimationController;
class Player : public GameObject
{
public:
	Player();
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	Transform* GetTransform();
private:
	int m_modelHandle;

	std::shared_ptr<AnimationController>m_animationController;
};

