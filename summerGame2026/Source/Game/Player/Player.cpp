#include "Player.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Core/InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Collision/CollisionManager.h"
#include"Engine/Collision/CapsuleCollider.h"
namespace
{
	constexpr float kSpeed = 5.0f;

	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());

	constexpr float kCapsuleHeightOffset = 10.0f;
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

	constexpr float kGroundRayHeightOffset = 20.0f;
	constexpr float kGroundRayLength = 25.0f;
	constexpr float kJumpPowor = 20.0f;
}

Player::Player(int playerModel, int stageModel, CameraManager& cameraManager) :
	Character(cameraManager),
	m_modelHandle(-1),
	m_stageModelHandle(-1),
	m_isGround(false),
	m_groundPlayerPos({})
{
	m_modelHandle = MV1DuplicateModel(playerModel);
	m_stageModelHandle = stageModel;

	m_collider = std::make_unique<CapsuleCollider>(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f }, kCapsuleRadius, kCapsuleHeight);
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
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
}

void Player::End()
{
}

void Player::Update()
{
	InputMove(kSpeed);
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

	if(m_moveInput.Length() == 0)
	{
		if (m_isGround)
		{
			m_animationController->Play(PlayerAnim::idle);
		}
	}

	Gravity();

	auto& input = InputManager::GetInstance();
	if (input.IsTriggered("A"))
	{
		if (m_isGround)
		{
			m_velocity.y = kJumpPowor;
			m_isGround = false;
			m_animationController->Play(PlayerAnim::jump,false);
		}
	}

	m_collider->Update(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f } + m_velocity);

	WallCollision(m_stageModelHandle);

	GroundCollision();

	ResetPlayerPos({0.0f,0.0f,0.0f});

	UpdateModel();

	m_animationController->Update();

//	m_capsuleCol.ReUpdate(m_transform.GetPosition());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_collider->Draw();
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

const Collider& Player::GetCollider() const
{
	return *m_collider;
}

const Ray& Player::GetRay() const
{
	return m_ray[0];
}

CameraAnchor Player::GetCameraAnchor() const
{
	return CameraAnchor{ m_transform, m_groundPlayerPos.y };
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

Transform* Player::GetTransform()
{
	return &m_transform;
}

Vector3 Player::GetGroundPlayerPos() const
{
	return m_groundPlayerPos;
}

void Player::GroundCollision()
{
	float groundHeight = -10000.0f;
	m_isGround = false;

	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i].Update(m_transform.GetPosition() + m_velocity + Vector3{ 0.0f,kGroundRayHeightOffset,0.0f } + kGroundRayOffsets[i]);
		auto rayInfo = m_ray[i].GetRayInfo();
		auto rayColInfo = CollisionManager::CheckCollRayAndPolygon(m_stageModelHandle, -1, rayInfo.start, rayInfo.end);

		if (rayColInfo.HitFlag)
		{
			if (groundHeight < rayColInfo.HitPosition.y)
			{
				groundHeight = rayColInfo.HitPosition.y;
			}

			if (groundHeight > -10000.0f)
			{
				m_transform.SetPosition(Vector3{ rayColInfo.HitPosition.x,groundHeight,rayColInfo.HitPosition.z } + -kGroundRayOffsets[i]);
				m_isGround = true;
				m_velocity = { 0.0f,0.0f,0.0f };
				m_groundPlayerPos = m_transform.GetPosition();
			}
		}
	}

	if (!m_isGround)
	{
		m_transform.Translate(m_velocity);
	}
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
