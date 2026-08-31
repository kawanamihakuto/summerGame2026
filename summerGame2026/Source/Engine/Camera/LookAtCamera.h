#pragma once
#include "CameraBase.h"
class LookAtCamera : public CameraBase
{
public:
	LookAtCamera(const Vector3& pos,const Vector3& target);
	//初期化
	void Init()override;
	//終了時処理
	void End()override;
	//更新
	void Update()override;
	//DxLibへの反映
	void Apply()override;
	//見るポジション
	void SetTarget(const Vector3& target);
	//自身のCameraNameを返す
	CameraName GetCameraName()override;
	
private:
	Vector3 m_targetPos;
};

