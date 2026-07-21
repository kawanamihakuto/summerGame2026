#pragma once
#include"Engine/Input/IControllable.h"
#include"Engine/Camera/ICameraTarget.h"

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
};

