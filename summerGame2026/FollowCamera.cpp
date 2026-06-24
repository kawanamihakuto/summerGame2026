#include "FollowCamera.h"
#include"Engine/Core/InputManager.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Player.h"
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
	auto& input = InputManager::GetInstance();

	Vector2 stick = input.GetRightStick();

	Vector3 rot = { m_transform.rotation + Vector3(-stick.y * kRotateSpeed,stick.x * kRotateSpeed,0.0f) }; 

	if (rot.x >= DX_PI_F / 3.0f)
	{
		rot.x = DX_PI_F / 3.0f;
	}
	if (rot.x <= -DX_PI_F / 3.0f)
	{
		rot.x = -DX_PI_F / 3.0f;
	}

	m_transform.SetRotate(rot);

	Matrix4x4 yawMat = Matrix4x4::RotationY(m_transform.rotation.y);
	Matrix4x4 pitchMat = Matrix4x4::RotationX(m_transform.rotation.x);

	Matrix4x4 rotMat = yawMat * pitchMat;

	Vector3 offset = rotMat.TransformVector(kOffset);

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
