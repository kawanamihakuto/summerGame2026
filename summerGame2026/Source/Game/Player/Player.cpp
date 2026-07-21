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
	constexpr float kGroundRayHeightOffset = 135.0f;
	constexpr float kGroundRayLength = 140.0f;
	constexpr float kGroundRayWidthOffset = 15.0f;
	
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
	m_isNextJump(false)
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
		if (m_isNextJump)
		{
			Jump();
		}

		UpdateMove();
		UpdateRotate(kModelRotationOffset);

		Gravity();

		Vector3 dir = { m_velocity.x, 0.0f, m_velocity.z };
		if (dir.Length() > 0.0f)
		{
			if (m_isGround)
			{
				m_animationController->Play(PlayerAnim::run);
			}
		}

		if (dir.Length() == 0)
		{
			if (m_isGround)
			{
				m_animationController->Play(PlayerAnim::idle);
			}
		}

		m_collider->Update(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f } + m_velocity);

		ResolveWallVelocity(m_stageModelHandle);

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
	}
}

void Player::Draw()
{
	if (m_isActive)
	{
		MV1DrawModel(m_modelHandle);
	}

#ifdef _DEBUG
	if (m_isActive)
	{
		m_collider->Draw();
		for (int i = 0; i < kGroundRayNum; i++)
		{
			m_ray[i]->Draw();
		}
	}

	DrawFormatString(16,96,0xffffff,L"isGround : %d",m_isGround);
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

void Player::OnCollision(ICollider& other, CollisionResult& result)
{
	if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
	{
		if (result.normal.y < -0.5f && m_velocity.y < 0.0f)
		{
			m_isNextJump = true;
		}
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

ICaptureTarget* Player::GetControllTarget()
{
	return this;
}

ICaptureTarget* Player::GetHatAndCameraTarget()
{
	return this;
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
	if (m_isGround || m_isNextJump)
	{
		m_velocity.y = kJumpPower;
		m_isGround = false;
		m_isNextJump = false;
		m_animationController->Play(PlayerAnim::jump, false);
	}
}

void Player::Controll()
{
	m_isActive = true;
}

void Player::ExitControll()
{
	m_isActive = false;
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
