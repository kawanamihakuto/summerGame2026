#pragma once
#include "CameraBase.h"
#include"Engine/Math/Transform.h"
#include"Engine/Camera/ICameraTarget.h"

class ICaptureTarget;
class Player;
class FollowCamera : public CameraBase
{
public:
	//コンストラクタで最初のターゲットをもらう
	FollowCamera(ICaptureTarget* target);
	//初期化
	void Init()override;
	//終了時処理
	void End()override;
	//更新
	void Update()override;
	//DxLibへの反映
	void Apply()override;
	/// <summary>
	/// Followするターゲットをセットする
	/// </summary>
	/// <param name="target">ターゲットのTransformポインタ</param>
	void SetTarget(ICaptureTarget* target);
	//自身のCameraNameを返す
	CameraName GetCameraName()override;
private:
	ICaptureTarget* m_target = nullptr;
	Player* m_player = nullptr;

	Vector3 m_pos;
	Vector3 m_targetPos;
	float m_groundPlayerHeight;

	float m_yaw;
	float m_pitch;
};

