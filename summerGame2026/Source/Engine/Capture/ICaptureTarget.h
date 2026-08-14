#pragma once
#include"Engine/Input/IControllable.h"
#include"Engine/Camera/ICameraTarget.h"

class GameObject;
/// <summary>
/// キャプチャーされるオブジェクトの抽象基底クラス(IControllableとICameraTargetを継承している)
/// </summary>
class ICaptureTarget : public IControllable,public ICameraTarget
{
public:
	virtual ~ICaptureTarget() = default;
	//帽子をかぶる位置
	virtual Matrix4x4 GetHatMatrix()const abstract;
	//PlayerControllerに渡すオブジェクト
	virtual ICaptureTarget* GetControllTarget()abstract;
	//帽子とフォローカメラに渡すオブジェクト
	virtual ICaptureTarget* GetHatAndCameraTarget()abstract;
	//GameObject型で渡す
	virtual GameObject* GetGameObject()abstract;
	//キャプチャー解除依頼があるか
	bool IsCaptureReleaseRequest() { return m_isCaptureReleaseRequest; }
	//キャプチャー解除依頼
	void SetCaptureRelease(bool is) { m_isCaptureReleaseRequest = is; }
private:
	bool m_isCaptureReleaseRequest = false;
};

