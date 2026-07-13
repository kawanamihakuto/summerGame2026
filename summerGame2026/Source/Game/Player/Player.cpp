#include "Player.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Collision/CollisionManager.h"
#include"Engine/Collision/CapsuleCollider.h"
namespace
{
	constexpr float kSpeed = 5.0f;
	constexpr float kMaxSpeed = 6.0f;

	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());

	constexpr float kCapsuleHeightOffset = 10.0f;
	constexpr float kCapsuleRadius = 30.0f;
	constexpr float kCapsuleHeight = 130.0f;

	constexpr int kGroundRayNum = 4;
	constexpr float kGroundRayHeightOffset = 20.0f;
	constexpr float kGroundRayWidthOffset = 15.0f;
	constexpr float kGroundRayLength = 25.0f;
	constexpr Vector3 kGroundRayOffsets[kGroundRayNum] =
	{
		{0.0f,kGroundRayHeightOffset,kGroundRayWidthOffset},
		{0.0f,kGroundRayHeightOffset,-kGroundRayWidthOffset},
		{kGroundRayWidthOffset,kGroundRayHeightOffset,0.0f},
		{-kGroundRayWidthOffset,kGroundRayHeightOffset,0.0f},
	};

	constexpr float kJumpPower = 20.0f;

	constexpr const wchar_t* kHeadFrameName = L"mixamorig:HeadTop_End";
}

Player::Player(int playerModel, int stageModel, CameraManager& cameraManager) :
	Character(cameraManager),
	m_modelHandle(-1),
	m_stageModelHandle(-1),
	m_groundPlayerPos({}),
	m_isActive(true)
{
	m_modelHandle = MV1DuplicateModel(playerModel);
	m_stageModelHandle = stageModel;

	m_collider = std::make_unique<CapsuleCollider>(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f }, kCapsuleRadius, kCapsuleHeight);

	m_ray.resize(kGroundRayNum);
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i] = std::make_unique<Ray>(m_transform.position, Vector3{ 0.0f,-1.0f,0.0f }, kGroundRayLength, kGroundRayOffsets[i]);
	}

	m_headFrameIndex = MV1SearchFrame(m_modelHandle, kHeadFrameName);
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	m_animationController = std::make_shared<AnimationController>(m_modelHandle);

	m_animationController->AddAnimation(PlayerAnim::idle);
	m_animationController->AddAnimation(PlayerAnim::run);
	m_animationController->AddAnimation(PlayerAnim::jump);

	m_animationController->Play(PlayerAnim::idle);
}

void Player::End()
{
}

void Player::Update()
{
	if (m_isActive)
	{
		UpdateMove();
		UpdateRotate(kModelRotationOffset);

		Vector3 dir = { m_velocity.x, 0.0f, m_velocity.z };
		if (dir.Length() > 0.0f)
		{
			if (m_isGround)
			{
				m_animationController->Play(PlayerAnim::run);
			}
		}

		if (m_velocity.Length() == 0)
		{
			if (m_isGround)
			{
				m_animationController->Play(PlayerAnim::idle);
			}
		}

		Gravity();

		m_collider->Update(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f } + m_velocity);

		WallCollision(m_stageModelHandle);

		for (auto& ray : m_ray)
		{
			ray->Update(m_transform.GetPosition() + m_velocity);
		}

		GroundCollision(m_stageModelHandle);

		if (!m_isGround)
		{
			m_transform.Translate(m_velocity);
		}

		ResetPlayerPos({ 0.0f,0.0f,0.0f });

		UpdateModel();

		m_animationController->Update();

		//	m_capsuleCol.ReUpdate(m_transform.GetPosition());
	}
}

void Player::Draw()
{
	if (m_isActive)
	{
		MV1DrawModel(m_modelHandle);
	}

#ifdef _DEBUG
	m_collider->Draw();
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i]->Draw();
	}

	DrawFormatString(16,96,0xffffff,L"isGround : %d",m_isGround);

	/*
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
		*/
#endif // _DEBUG
}

const Collider& Player::GetCollider() const
{
	return *m_collider;
}

const Ray& Player::GetRay() const
{
	return *m_ray[0];
}

CameraAnchor Player::GetCameraAnchor() const
{
	return CameraAnchor{m_transform};
}

CollisionLayer Player::GetCollisionLayer() const
{
	return CollisionLayers::kPlayer;
}

CollisionLayer Player::GetCollisionMask() const
{
	return CollisionLayers::kEnemy;
}

void Player::OnCollision(ICollider& other)
{
	if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
	{
#ifdef _DEBUG
		DrawFormatString(16,300,0xffffff,L"敵にヒット");
#endif // _DEBUG
	}
}

Vector3 Player::GetPosition() const
{
	return m_transform.position;
}

Matrix4x4 Player::GetHatMatrix() const
{
	Matrix4x4 mat = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_headFrameIndex);

	Matrix4x4 offsetMat = Matrix4x4::Translate({ 0.0f,-10.0f,-10.0f });

	mat*= offsetMat;

	return mat;
}

void Player::Move(const Vector2& input)
{	
	auto& camera = m_cameraManager.GetTransfrom();
	//移動ベクトル生成
	m_moveInput = Vector3::Zero();
	m_moveInput += Vector3{ camera.Right().x,0.0f,camera.Right().z } * input.x;
	m_moveInput += Vector3{ camera.Forward().x,0.0f,camera.Forward().z } * input.y;
	m_moveInput.Normalize();
	m_moveInput.x *= kSpeed;
	m_moveInput.z *= kSpeed;
	m_moveInput.y = 0.0f;

	if (m_moveInput.Length() > 0.0f)
	{
		m_lastMoveInput = m_moveInput;
	}
}

void Player::Jump()
{
	if (m_isGround)
	{
		m_velocity.y = kJumpPower;
		m_isGround = false;
		m_animationController->Play(PlayerAnim::jump, false);
	}
}

Transform* Player::GetTransform()
{
	return &m_transform;
}

Vector3 Player::GetGroundPlayerPos() const
{
	return m_groundPlayerPos;
}

void Player::UpdateModel()
{
	Matrix4x4 worldMat = m_transform.GetWorldMatrix();

	MV1SetMatrix(m_modelHandle, worldMat.ChangeDxMat());
}

void Player::ResetPlayerPos(const Vector3& pos)
{
	if (m_transform.GetPosition().y <= -10000.0f)
	{
		m_transform.SetPosition(pos);
	}
}

void Player::CaptureReleaseAction(const Vector3& pos)
{
	m_transform.SetPosition(pos);
	m_velocity.x = GetRand(2) - 1.0f;
	m_velocity.x *= 6.0f;
	m_velocity.z = GetRand(2) - 1.0f;
	m_velocity.z *= 6.0f;
	m_velocity.y = kJumpPower;
	m_isGround = false;
	m_animationController->Play(PlayerAnim::jump, false);
}
