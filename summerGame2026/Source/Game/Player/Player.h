#pragma once
#include"Engine/Core/GameObject.h"
#include"Engine/Collision/CapsuleCollider.h"

struct PlayerAnim
{
	std::wstring idle = L"Armature|Idle";
	std::wstring run = L"Armature|Run";
};

class AnimationController;
class Player : public GameObject
{
public:
	Player(int playerModel,int stageModel);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	Transform* GetTransform();
private:
	int m_modelHandle;

	int m_stageModelHandle;

	std::shared_ptr<AnimationController>m_animationController;

	PlayerAnim m_anim;

	CapsuleCollider m_capsuleCol;
};

