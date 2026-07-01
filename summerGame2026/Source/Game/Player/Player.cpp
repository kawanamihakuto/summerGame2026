#include "Player.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Core/InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Collision/CollisionManager.h"
namespace
{
	constexpr float kSpeed = 5.0f;

	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());

	constexpr float kCapsuleHeightOffset = 5.0f;
	constexpr float kCapsuleRadius = 30.0f;
	constexpr float kCapsuleHeight = 130.0f;

	constexpr int kGroundRayNum = 4;

	constexpr Vector3 kGroundRayOffsets[kGroundRayNum] =
	{
		{0.0f,0.0f,15.0f},
		{0.0f,0.0f,-15.0f},
		{15.0f,0.0f,0.0f},
		{-15.0f,0.0f,0.0f},
	};

	constexpr float kGroundRayHeightOffset = 100.0f;
	constexpr float kGroundRayLength = 105.0f;

	constexpr float kGravity = 1.0f;

	constexpr float kJumpPowor = 20.0f;
}

Player::Player(int playerModel, int stageModel) :
	m_modelHandle(-1),
	m_stageModelHandle(-1),
	m_velocity({}),
	m_isGround(false)
{
	m_modelHandle = MV1DuplicateModel(playerModel);
	m_stageModelHandle = stageModel;
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	m_capsuleCol.Init(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f }, kCapsuleRadius, kCapsuleHeight);

	m_ray.resize(kGroundRayNum);

	for (int i = 0; i < 4; i++)
	{
		m_ray[i].Init(m_transform.GetPosition() + Vector3{ 0.0f,kGroundRayHeightOffset,0.0f } + kGroundRayOffsets[i], { 0.0f,-1.0f,0.0f }, kGroundRayLength);
	}

	m_animationController = std::make_shared<AnimationController>(m_modelHandle);

	m_animationController->AddAnimation(PlayerAnim::idle);
	m_animationController->AddAnimation(PlayerAnim::run);
	m_animationController->AddAnimation(PlayerAnim::jump);

	m_animationController->Play(PlayerAnim::idle);

	int test = MV1SetupCollInfo(m_stageModelHandle, -1, 8, 8, 8);

	m_velocity = { 0.0f,0.0f,0.0f };
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
	move += Vector3{cameraTransform.Right().x,0.0f,cameraTransform.Right().z} *leftStick.x;
	move += Vector3{cameraTransform.Forward().x,0.0f,cameraTransform.Forward().z} * leftStick.y;
	move.Normalize();
	move.x *= kSpeed;
	move.z *= kSpeed;
	move.y = 0.0f;
	m_velocity.x = move.x;
	m_velocity.z = move.z;
	//スティックが倒されてる時だけ回転
	if (move.Length() > 0.0f)
	{
		if(m_isGround)
		{
			m_animationController->Play(PlayerAnim::run);
		}

		Vector3 dir = move.Normalized();

		DrawFormatString(16, 150, 0xffffff, L"dir : %f,%f,%f", dir.x, dir.y, dir.z);

		//目標の回転を作成
		Quaternion targetRot = Quaternion::LookRotation(dir, Vector3::Up());
		targetRot = targetRot * kModelRotationOffset;

		//現在の回転から補間
		Quaternion rot = Quaternion::Slerp(m_transform.GetRotation(), targetRot, 0.15f);

		m_transform.SetRotate(rot);
	}
	else
	{
		if (m_isGround)
		{
			m_animationController->Play(PlayerAnim::idle);
		}
	}

	if (input.IsTriggered("A"))
	{
		if (m_isGround)
		{
			m_velocity.y = kJumpPowor;
			m_isGround = false;
			m_animationController->Play(PlayerAnim::jump,false);
		}
	}

	m_velocity.y -= kGravity;

	m_capsuleCol.Update(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f } + move);

	auto capsuleInfo = m_capsuleCol.GetCapsuleInfo();
	auto capColInfo = CollisionManager::CheckCollCapsuleAndPolygon(m_stageModelHandle, -1, capsuleInfo.start, capsuleInfo.end, kCapsuleRadius);

	if (capColInfo.HitNum > 0)
	{
		for (int i = 0; i < capColInfo.HitNum; i++)
		{
			auto& poly = capColInfo.Dim[i];

			Vector3 normal = { poly.Normal.x,poly.Normal.y,poly.Normal.z };
			normal.Normalize();

			float dot = move.Dot(normal);

			if (dot < 0.0f)
			{
				move -= normal * dot;
				m_capsuleCol.Hit();
			}
		}
	}

	//解放
	MV1CollResultPolyDimTerminate(capColInfo);

	m_velocity = { move.x,m_velocity.y,move.z };

	int hitCount = 0;
	m_isGround = false;

	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i].Update(m_transform.GetPosition() + m_velocity + Vector3{ 0.0f,kGroundRayHeightOffset,0.0f } + kGroundRayOffsets[i]);
		auto rayInfo = m_ray[i].GetRayInfo();
		auto rayColInfo = CollisionManager::CheckCollRayAndPolygon(m_stageModelHandle, -1, rayInfo.start, rayInfo.end);

		if (rayColInfo.HitFlag)
		{
			hitCount++;

			if (hitCount >= 1)
			{
				m_transform.SetPosition(Vector3{ rayColInfo.HitPosition.x,rayColInfo.HitPosition.y,rayColInfo.HitPosition.z } + -kGroundRayOffsets[i]);
				m_isGround = true;
				m_velocity = { 0.0f,0.0f,0.0f };
				break;
			}
		}
	}

	if (!m_isGround)
	{
		m_transform.Translate(m_velocity);
	}

	Matrix4x4 worldMat = m_transform.GetWorldMatrix();

	MV1SetMatrix(m_modelHandle, worldMat.ChangeDxMat());

	m_animationController->Update();

	m_capsuleCol.ReUpdate(m_transform.GetPosition());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_capsuleCol.Draw();
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i].Draw();
	}
	DrawFormatString(16, 32, 0xffffff, L"pos : %f,%f,%f", m_transform.position.x, m_transform.position.y, m_transform.position.z);
	DrawFormatString(16, 48, 0xffffff, L"scale : %f,%f,%f", m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);

	Matrix4x4 mat = Matrix4x4::RotationY(DX_PI_F / 2);
	Vector3 v1 = mat.TransformVector({ 0,0,1 });
	DrawFormatString(16, 184, 0xffffff, L"v : %f,%f,%f", v1.x, v1.y, v1.z);

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
#endif // _DEBUG
}

Transform* Player::GetTransform()
{
	return &m_transform;
}
