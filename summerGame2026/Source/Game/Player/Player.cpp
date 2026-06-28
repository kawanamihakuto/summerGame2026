#include "Player.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Core/InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Animation/AnimationController.h"

namespace
{
	const wchar_t* kModelPath = L"data/model/Player.mv1";
	constexpr float kSpeed = 5.0f;
	
	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());
}

Player::Player() :
	m_modelHandle(-1)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_modelHandle = MV1LoadModel(kModelPath);

	m_animationController = std::make_shared<AnimationController>(m_modelHandle);

	m_animationController->AddAnimation(m_anim.idle);

	m_animationController->Play(m_anim.idle);
}

void Player::End()
{
}

void Player::Update()
{
	//入力を取得
	auto& input = InputManager::GetInstance();
	auto& cameraTransform = CameraManager::GetInstance().GetTransfrom();
	Vector2 leftStick = input.GetLeftStick();
	//移動ベクトル生成
	Vector3 move = { 0.0f,0.0f,0.0f };
	move += cameraTransform.Right() * leftStick.x * kSpeed;
	move += cameraTransform.Forward() * leftStick.y * kSpeed;
	move.y = 0.0f;

	//スティックが倒されてる時だけ回転
	if (move.Length() > 0.0f)
	{
		Vector3 dir = move.Normalized();

		DrawFormatString(16, 150, 0xffffff, L"dir : %f,%f,%f", dir.x, dir.y, dir.z);

		//目標の回転を作成
		Quaternion targetRot = Quaternion::LookRotation(dir, Vector3::Up());
		targetRot = targetRot * kModelRotationOffset;

		//現在の回転から補間
		Quaternion rot = Quaternion::Slerp(m_transform.GetRotation(), targetRot, 0.15f);

		m_transform.SetRotate(rot);
	}

	m_transform.Translate(move);

	Matrix4x4 worldMat = m_transform.GetWorldMatrix();

	MV1SetMatrix(m_modelHandle, worldMat.ChangeDxMat());

	m_animationController->Update();
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
	DrawFormatString(16,32,0xffffff,L"pos : %f,%f,%f", m_transform.position.x, m_transform.position.y, m_transform.position.z);
	DrawFormatString(16,48,0xffffff,L"scale : %f,%f,%f", m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);

	Matrix4x4 mat = Matrix4x4::RotationY(DX_PI_F / 2);
	Vector3 v1 = mat.TransformVector({ 0,0,1 });
	DrawFormatString(16, 184, 0xffffff, L"v : %f,%f,%f",v1.x, v1.y, v1.z);

	Quaternion q = Quaternion::AngleAxis(DX_PI_F / 2, Vector3::Up());
	Vector3 v2 = q.ToMatrix().TransformVector({ 0,0,1 });
	DrawFormatString(16, 200, 0xffffff, L"v : %f,%f,%f", v2.x, v2.y, v2.z);
	
	Quaternion a =
		Quaternion::LookRotation(
			Vector3::Forward(),
			Vector3::Up());

	Vector3 f = a.Rotate(Vector3::Forward());

	DrawFormatString(16, 216, 0xffffff, L"v : %f,%f,%f", f.x, f.y, f.z);

	Quaternion b = Quaternion::AngleAxis(DX_PI_F / 2.0f, Vector3::Up());
	Matrix4x4 m = b.ToMatrix();

	DrawFormatString(16, 248, 0xffffff, L"m : %f,%f,%f\n%f,%f,%f\n%f,%f,%f",
		m.m00, m.m01, m.m02,
		m.m10, m.m11, m.m12,
		m.m20, m.m21, m.m22);
}

Transform* Player::GetTransform()
{
	return &m_transform;
}
