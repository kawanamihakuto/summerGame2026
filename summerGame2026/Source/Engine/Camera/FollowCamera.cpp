#include "FollowCamera.h"
#include"Engine/Core/InputManager.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Game/Player/Player.h"
namespace
{
	//カメラの後方オフセット
	constexpr Vector3 kOffset = { 0.0f,100.0f,-500.0f };
	constexpr float kRotateSpeed = 0.03f;
}

void FollowCamera::Init()
{
	auto player = GameObjectManager::GetInstance().Find<Player>();

	SetTarget(player->GetTransform());
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
	m_transform.SetPosition(m_target->position + offset);
}

void FollowCamera::Apply()
{
	SetCameraPositionAndTarget_UpVecY(
		m_transform.position.ChangeDxVector(),
		m_target->position.ChangeDxVector()
	);
}

void FollowCamera::SetTarget(Transform* target)
{
	m_target = target;
}
