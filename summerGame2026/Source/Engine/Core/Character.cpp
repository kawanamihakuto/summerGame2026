#include "Character.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Collision/Collider.h"
#include"Engine/Collision/Ray.h"

namespace
{
	constexpr float kGravity = 1.0f;
}

Character::Character(CameraManager& cameraManager) :
	m_cameraManager(cameraManager)
{
}

void Character::InputMove(float speed)
{
	//入力を取得
	auto& input = InputManager::GetInstance();
	auto& camera = m_cameraManager.GetTransfrom();
	Vector2 leftStick = input.GetLeftStick();
	//移動ベクトル生成
	m_moveInput = Vector3::Zero();
	m_moveInput += Vector3{ camera.Right().x,0.0f,camera.Right().z } * leftStick.x;
	m_moveInput += Vector3{ camera.Forward().x,0.0f,camera.Forward().z } * leftStick.y;
	m_moveInput.Normalize();
	m_moveInput.x *= speed;
	m_moveInput.z *= speed;
	m_moveInput.y = 0.0f;
}

void Character::UpdateMove()
{
	m_velocity.x = m_moveInput.x;
	m_velocity.z = m_moveInput.z;
}

void Character::UpdateRotate(Quaternion rotationOffset)
{
	Vector3 dir = { m_velocity.x, 0.0f, m_velocity.z };
	//スティックが倒されてる時だけ回転
	if (dir.Length() > 0.0f)
	{
		dir.Normalize();

		DrawFormatString(16, 150, 0xffffff, L"dir : %f,%f,%f", dir.x, dir.y, dir.z);

		//目標の回転を作成
		Quaternion targetRot = Quaternion::LookRotation(dir, Vector3::Up());
		targetRot = targetRot * rotationOffset;

		//現在の回転から補間
		Quaternion rot = Quaternion::Slerp(m_transform.GetRotation(), targetRot, 0.15f);

		m_transform.SetRotate(rot);
	}
}
