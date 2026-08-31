#include "LookAtCamera.h"

LookAtCamera::LookAtCamera(const Vector3& pos, const Vector3& target) :
	m_targetPos(target)
{
	m_transform.SetPosition(pos);
}

void LookAtCamera::Init()
{

}

void LookAtCamera::End()
{
}

void LookAtCamera::Update()
{
	
}

void LookAtCamera::Apply()
{
	//DxLibに反映
	SetCameraPositionAndTarget_UpVecY(
		m_transform.position,
		m_targetPos
	);

	//ライトの方向を適用(仮)
	SetLightDirection(m_transform.Forward());
}

void LookAtCamera::SetTarget(const Vector3& target)
{
	//ターゲット設定
	m_targetPos = target;
}

CameraName LookAtCamera::GetCameraName()
{
	//フォローカメラです
	return CameraName::lookat;
}


