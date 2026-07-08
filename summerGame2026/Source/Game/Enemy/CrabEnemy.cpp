#include "CrabEnemy.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Quaternion.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Camera/CameraManager.h"
namespace
{
	constexpr Vector3 kSphereOffset = { 0.0f,50.0f,0.0f };
	constexpr float kSpeed = 2.0f;
	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());
	constexpr Vector3 kScale = { 0.5f,0.5f,0.5f };
	constexpr float kRadius = 50.0f;

	constexpr int kGroundRayNum = 4;
	constexpr float kGroundRayHeightOffset = 10.0f;
	constexpr float kGroundRayWidthOffset = 15.0f;
	constexpr float kGroundRayLength = 15.0f;
	constexpr Vector3 kGroundRayOffsets[kGroundRayNum] =
	{
		{0.0f,kGroundRayHeightOffset,kGroundRayWidthOffset},
		{0.0f,kGroundRayHeightOffset,-kGroundRayWidthOffset},
		{kGroundRayWidthOffset,kGroundRayHeightOffset,0.0f},
		{-kGroundRayWidthOffset,kGroundRayHeightOffset,0.0f},
	};

	constexpr float kJumpPower = 10.0f;
}

CrabEnemy::CrabEnemy(int enemyModel, int stageModel, CameraManager& camera) :
	Character(camera)
{
	m_modelHandle = MV1DuplicateModel(enemyModel);
	m_stageModelHandle = stageModel;

	m_collider = std::make_unique<SphereCollider>(m_transform.position, kRadius);

	m_ray.resize(kGroundRayNum);
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i] = std::make_unique<Ray>(m_transform.position, Vector3{ 0.0f,-1.0f,0.0f }, kGroundRayLength, kGroundRayOffsets[i]);
	}
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

	m_transform.SetPosition({ 0.0f,-200.0f,0.0f });

	MV1SetScale(m_modelHandle,kScale);
}

void CrabEnemy::End()
{
}

void CrabEnemy::Update()
{
	if (!m_isControll)
	{
		if (m_target != nullptr)
		{
			Vector3 vec = m_target->GetPosition() - m_transform.position;
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
		}

		m_velocity.x *= kSpeed;
		m_velocity.z *= kSpeed;
	}
	else
	{
		UpdateMove();
	}
	
	UpdateRotate(kModelRotationOffset);

	Gravity();

	m_collider->Update(m_transform.position + kSphereOffset);

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

	MV1SetPosition(m_modelHandle, m_transform.GetPosition());
	MV1SetRotationMatrix(m_modelHandle, m_transform.GetRotationMatrix().ChangeDxMat());
	m_animationController->Update();
}

void CrabEnemy::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_collider->Draw();
#endif // _DEBUG

}

void CrabEnemy::SetTarget(const ITarget* target)
{
	m_target = target;
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
	return CollisionLayers::kPlayer;
}

void CrabEnemy::OnCollision(ICollider& other)
{
	if (other.GetCollisionLayer() == CollisionLayers::kPlayer)
	{
#ifdef _DEBUG
		DrawFormatString(16, 316, 0xffffff, L"プレイヤーにヒット");
#endif // _DEBUG
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
	m_moveInput.x *= kSpeed;
	m_moveInput.z *= kSpeed;
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
