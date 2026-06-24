#pragma once
#include "CameraBase.h"
#include"Engine/Math/Transform.h"
class FollowCamera : public CameraBase
{
public:
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
	void SetTarget(Transform* target);

private:
	Transform* m_target = nullptr;
};

