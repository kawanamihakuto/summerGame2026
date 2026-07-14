#include "CrabEnemy.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Quaternion.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Capture/CaptureManager.h"
#include"Engine/Collision/CollisionManager.h"
namespace
{
	constexpr Vector3 kSphereOffset = { 0.0f,50.0f,0.0f };
	constexpr float kSpeed = 2.0f;
	constexpr float kCapturedSpeed = 4.0f;
	constexpr float kMaxSpeed = 3.0f;
	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());
	constexpr Vector3 kScale = { 0.5f,0.5f,0.5f };
	constexpr float kRadius = 40.0f;

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

	constexpr float kJumpPower = 16.0f;

	constexpr const wchar_t* kHeadFrameName = L"Head3";
}

CrabEnemy::CrabEnemy(int enemyModel, int stageModel, CameraManager& camera, CaptureManager& captureManager,const Vector3& pos) :
	Character(camera),
	m_captureManager(captureManager)
{
	m_modelHandle = MV1DuplicateModel(enemyModel);
	m_stageModelHandle = stageModel;

	m_collider = std::make_unique<SphereCollider>(m_transform.position, kRadius);

	m_ray.resize(kGroundRayNum);
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i] = std::make_unique<Ray>(m_transform.position, Vector3{ 0.0f,-1.0f,0.0f }, kGroundRayLength, kGroundRayOffsets[i]);
	}

	m_HeadFrameIndex = MV1SearchFrame(m_modelHandle, kHeadFrameName);

	m_transform.SetPosition(pos);
}

CrabEnemy::~CrabEnemy()
{
	MV1DeleteModel(m_modelHandle);
}

void CrabEnemy::Init()
{
	m_animationController = std::make_shared<AnimationController>(m_modelHandle);
	m_animationController->AddAnimation(CrabEnemyAnim::idle);
	m_animationController->AddAnimation(CrabEnemyAnim::walk);
	m_animationController->AddAnimation(CrabEnemyAnim::jump);
	m_animationController->Play(CrabEnemyAnim::idle);

	MV1SetScale(m_modelHandle, kScale);
}

void CrabEnemy::End()
{
}

void CrabEnemy::Update()
{
	if (!m_isControll)
	{
		Vector3 vec = m_captureManager.GetTarget()->GetHatMatrix().GetTranslation() - m_transform.position;
		vec.y = 0.0f;
		if (vec.Length() > 1.0f)
		{
			vec.Normalize();
			m_animationController->Play(CrabEnemyAnim::walk);
		}
		else
		{
			vec = Vector3::Zero();
			m_animationController->Play(CrabEnemyAnim::idle);
		}

		m_velocity.x = vec.x;
		m_velocity.z = vec.z;

		m_velocity.x *= kSpeed;
		m_velocity.z *= kSpeed;
	}
	else
	{
		Vector3 dir = m_moveInput;
		if (dir.Length() > 0.0f)
		{
			if (m_isGround)
			{
				m_animationController->Play(CrabEnemyAnim::walk);
			}
		}

		if (m_moveInput.Length() == 0)
		{
			if (m_isGround)
			{
				m_animationController->Play(CrabEnemyAnim::idle);
			}
		}

		UpdateMove();
	}

	UpdateRotate(kModelRotationOffset);

	Gravity();

	m_collider->Update(m_transform.position + kSphereOffset + m_velocity);

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

	ResetEnemyPos({ 0.0f,0.0f,0.0f });

	MV1SetPosition(m_modelHandle, m_transform.GetPosition());
	MV1SetRotationMatrix(m_modelHandle, m_transform.GetRotationMatrix().ChangeDxMat());
	m_animationController->Update();
}

void CrabEnemy::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_collider->Draw();
	for (auto& ray : m_ray)
	{
		ray->Draw();
	}

#endif // _DEBUG

}

void CrabEnemy::ResetEnemyPos(const Vector3& pos)
{
	if (m_transform.GetPosition().y <= -10000.0f)
	{
		m_transform.SetPosition(pos);
	}
}

const Collider& CrabEnemy::GetCollider() const
{
	return *m_collider;
}

const Ray& CrabEnemy::GetRay() const
{
	return *m_ray[0];
}

CollisionLayer CrabEnemy::GetCollisionLayer() const
{
	return CollisionLayers::kEnemy;
}

CollisionLayer CrabEnemy::GetCollisionMask() const
{
	return CollisionLayers::kPlayer |
		CollisionLayers::kHat|
		CollisionLayers::kEnemy;
}

void CrabEnemy::OnCollision(ICollider& other, CollisionResult& result)
{
	if (other.GetCollisionLayer() == CollisionLayers::kPlayer)
	{
#ifdef _DEBUG
		DrawFormatString(16, 316, 0xffffff, L"プレイヤーにヒット");
#endif // _DEBUG
	}
	
	if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
	{
		Vector3 push = result.normal * result.penetration;

		auto hits = m_collider->CheckWallCollision(m_stageModelHandle);

		for (const auto& hit : hits)
		{
			if (hit.normal.y > 0.7f)
				continue;

			float dot = push.Dot(hit.normal);

			if (dot < 0.0f)
			{
				push -= hit.normal * dot;
			}
		}

		m_transform.Translate(push);
	}
}

void CrabEnemy::Move(const Vector2& input)
{
	auto& camera = m_cameraManager.GetTransfrom();
	//移動ベクトル生成
	m_moveInput = Vector3::Zero();
	m_moveInput += Vector3{ camera.Right().x,0.0f,camera.Right().z } * input.x;
	m_moveInput += Vector3{ camera.Forward().x,0.0f,camera.Forward().z } * input.y;
	m_moveInput.Normalize();
	m_moveInput.x *= kCapturedSpeed;
	m_moveInput.z *= kCapturedSpeed;
	m_moveInput.y = 0.0f;
}

void CrabEnemy::Jump()
{

	if (m_isGround)
	{
		m_velocity.y = kJumpPower;
		m_isGround = false;
		m_animationController->Play(CrabEnemyAnim::jump, false);
	}
}

CameraAnchor CrabEnemy::GetCameraAnchor() const
{
	return { m_transform };
}

Matrix4x4 CrabEnemy::GetHatMatrix() const
{
	Matrix4x4 mat = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_HeadFrameIndex);
	return mat;
}
