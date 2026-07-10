#include "Hat.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Capture/ICaptureTarget.h"
#include"Engine/Math/Transform.h"
#include"Engine/Math/Quaternion.h"

namespace
{
	constexpr float kGoSpeed = 14.0f;
	constexpr float kBackSpeed = 20.0f;
	constexpr float kLength = 200.0f;

	constexpr float kSphereRadius = 30.0f;

	constexpr Vector3 kModelScale = { 60.0f,60.0f,60.0f };

	constexpr int kGoFrame = 35.0f;
	constexpr int kWaitFrame = 65.0f;
}

Hat::Hat(int modelHandle, int stageModelHandle, ICaptureTarget* target) :
	m_targetPos({}),
	m_target(target),
	m_endPos({}),
	m_count(0),
	m_direction({}),
	m_CaptureFlag(false)
{
	m_modelHandle = MV1DuplicateModel(modelHandle);
	m_stageModelHandle = stageModelHandle;

	m_collider = std::make_unique<SphereCollider>(m_transform.position, kSphereRadius);
	m_collider->SetIsActive(false);
	m_state = HatState::have;
}

Hat::~Hat()
{
	MV1DeleteModel(m_modelHandle);
}

void Hat::Init()
{
	m_transform = Transform::FromMatrix(m_target->GetHatMatrix());
}

void Hat::End()
{
}

void Hat::Update()
{
	switch (m_state)
	{
	case HatState::have:
		m_transform = Transform::FromMatrix(m_target->GetHatMatrix());
		m_collider->SetIsActive(false);
		break;
	case HatState::go:
		m_direction.Normalize();
		m_velocity = m_direction * kGoSpeed;

		m_collider->Update(m_transform.position + m_velocity);

		WallCollision(m_stageModelHandle);

		m_transform.Translate(m_velocity);

		if (m_count++ >= kGoFrame)
		{
			m_state = HatState::wait;
			m_count = 0;
		}
		break;
	case HatState::wait:

		if (m_count++ >= kWaitFrame)
		{
			m_state = HatState::back;
			m_count = 0.0f;
		}
		break;
	case HatState::back:
		
		m_direction = m_target->GetHatMatrix().GetTranslation() - m_transform.position;
		if (m_direction.Length() <kBackSpeed)
		{
			m_state = HatState::have;
			m_count = 0;
			break;
		}
		m_direction.Normalize();
		m_velocity = m_direction * kBackSpeed;

		m_collider->Update(m_transform.position + m_velocity);

		WallCollision(m_stageModelHandle);

		m_transform.Translate(m_velocity);
		break;
	}
	
	MV1SetPosition(m_modelHandle,m_transform.position);
	MV1SetRotationMatrix(m_modelHandle, m_transform.rotation.ToMatrix().ChangeDxMat());
	MV1SetScale(m_modelHandle, kModelScale);
}

void Hat::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_collider->Draw();
	DrawFormatString(16,232,0xffffff,L"hatState : %d",m_state);
#endif // _DEBUG

}

void Hat::Throw(const Vector3& dir)
{
	if (m_state == HatState::have)
	{
		m_direction = -dir.Normalized();
		m_collider->SetIsActive(true);
		m_state = HatState::go;
		m_count = 0;
	}
}

const Collider& Hat::GetCollider() const
{
	return *m_collider;
}

const Ray& Hat::GetRay() const
{
	return *m_ray[0];
}

CollisionLayer Hat::GetCollisionLayer() const
{
	return CollisionLayers::kHat;
}

CollisionLayer Hat::GetCollisionMask() const
{
	return CollisionLayers::kEnemy;
}

void Hat::OnCollision(ICollider& other)
{
	if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
	{
		if (auto cast = dynamic_cast<ICaptureTarget*>(&other))
		{
			SetTarget(cast);

			m_CaptureFlag = true;
		}
	}
}

void Hat::SetTarget(ICaptureTarget* target)
{
	m_target = target;
}

ICaptureTarget* Hat::GetCaptureTarget()
{
	return m_target;
}
