#pragma once
#include"Engine/Core/GameObject.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Capture/ICaptureTarget.h"

class AnimationController;
class TitlePlayer : public GameObject , public ICaptureTarget
{
public:
	TitlePlayer(int model);
	~TitlePlayer();
	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	//----------------------------
	//ICaptureTargetの関数
	//----------------------------
	Matrix4x4 GetHatMatrix()const override;
	ICaptureTarget* GetControllTarget()override;
	ICaptureTarget* GetHatAndCameraTarget()override;
	GameObject* GetGameObject()override;

	//-----------------------------
	//ICameraTargetの関数
	//-----------------------------
	CameraAnchor GetCameraAnchor() const override;

	//----------------------------
	//IControllableの関数
	//----------------------------
	void Move()override;
	void Jump()override;
	void JumpEnd()override;
	void Controll()override;
	void ExitControll()override;
private:
	int m_modelHandle;

	std::shared_ptr<AnimationController> m_animationController;

	int m_headFrameIndex;
};

