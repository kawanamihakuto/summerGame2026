#include "FollowCamera.h"
#include"Engine/Core/InputManager.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Game/Player/Player.h"
namespace
{
	//カメラの後方オフセット
	constexpr Vector3 kOffset = { 0.0f,100.0f,-500.0f };
	constexpr Vector3 kCameraHeightOffset = { 0.0f,200.0f,0.0f };
	constexpr float kRotateSpeed = 0.03f;
}

FollowCamera::FollowCamera(ICameraTarget* target):
	m_yaw(0.0f),
	m_pitch(0.0f),
	m_groundPlayerHeight(0.0f),
	m_pos({}),
	m_targetPos({})
{
	SetTarget(target);
}

void FollowCamera::Init()
{
	
}

void FollowCamera::End()
{
}

void FollowCamera::Update()
{
	//入力を取得
	auto& input = InputManager::GetInstance();
	Vector2 stick = input.GetRightStick();
	//yawとpitch
	m_yaw += stick.x * kRotateSpeed;
	m_pitch -= stick.y * kRotateSpeed;

	//picthを下限と上限で補正
	m_pitch = std::clamp(m_pitch, -DX_PI_F / 3.0f, DX_PI_F / 3.0f);
	//クォータニオン作成
	Quaternion yawRot = Quaternion::AngleAxis(m_yaw, Vector3::Up());
	Quaternion pitchRot = Quaternion::AngleAxis(m_pitch, Vector3::Right());
	Quaternion rot = yawRot * pitchRot;

	//回転を適用
	m_transform.SetRotate(rot);
	//オフセットを回転
	Vector3 offset = rot.Rotate(kOffset);
	//ポジションを適用
	m_groundPlayerHeight = std::lerp(m_groundPlayerHeight, m_target->GetCameraAnchor().groundHeight, 0.06f);

	if (m_groundPlayerHeight > m_target->GetCameraAnchor().groundHeight)
	{
		m_groundPlayerHeight = m_target->GetCameraAnchor().groundHeight;
	}

	m_pos.x = std::lerp(m_pos.x, m_target->GetCameraAnchor().transform.GetPosition().x, 0.06f);
	m_pos.y = std::lerp(m_pos.y, m_target->GetCameraAnchor().transform.GetPosition().y, 0.03f);
	m_pos.z = std::lerp(m_pos.z, m_target->GetCameraAnchor().transform.GetPosition().z, 0.06f);

	m_transform.SetPosition(m_pos + offset);

	m_targetPos.x = std::lerp(m_targetPos.x, m_target->GetCameraAnchor().transform.GetPosition().x, 0.06f);
	m_targetPos.y = std::lerp(m_targetPos.y, m_target->GetCameraAnchor().transform.GetPosition().y, 0.06f);
	m_targetPos.z = std::lerp(m_targetPos.z, m_target->GetCameraAnchor().transform.GetPosition().z, 0.06f);
}

void FollowCamera::Apply()
{
	SetCameraPositionAndTarget_UpVecY(
		m_transform.position,
		m_targetPos+kCameraHeightOffset
	);
}

void FollowCamera::SetTarget(ICameraTarget* target)
{
	m_target = target;
}

CameraName FollowCamera::GetCameraName()
{
	return CameraName::follow;
}
