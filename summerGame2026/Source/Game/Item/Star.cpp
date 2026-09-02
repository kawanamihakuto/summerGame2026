#include "Star.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Core/EffectManager.h"
#include"Engine/Core/ResourceManager.h"

namespace
{
	constexpr float kSphereRadius = 60.0f;
}

Star::Star(int modelHandle, const Vector3& pos):
	m_frameCount(0),
	m_yaw(0.0f)
{
	m_modelHandle = MV1DuplicateModel(modelHandle);
	m_transform.SetPosition(pos);
	m_bodyCollider = std::make_unique<SphereCollider>(m_transform.position, kSphereRadius);
	m_bodyCollider->Update(m_transform.position);
	MV1SetPosition(m_modelHandle, m_transform.position);
}

Star::~Star()
{

}

void Star::Init()
{
	
}

void Star::End()
{
	MV1DeleteModel(m_modelHandle);
}

void Star::Update()
{
	m_frameCount++;
	m_transform.position.y += sinf(static_cast<float>(m_frameCount) * 0.05f);
	m_yaw += 0.05f;

	m_bodyCollider->Update(m_transform.position);
	MV1SetPosition(m_modelHandle,m_transform.position);
	MV1SetRotationXYZ(m_modelHandle, Vector3{ 0.0f,m_yaw,0.0f });
}

void Star::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_bodyCollider->Draw();
#endif // _DEBUG

}

const Collider& Star::GetCollider() const
{
	return *m_bodyCollider;
}

//const Ray& Star::GetRay() const
//{
//	return 
//}

CollisionLayer Star::GetCollisionLayer() const
{
	return CollisionLayers::kStar;
}

CollisionLayer Star::GetCollisionMask() const
{
	return CollisionLayers::kPlayer |
		CollisionLayers::kControllEnemy;
}

void Star::OnCollision(ICollider& other, CollisionResult& result)
{
	EffectManager::Play("getStar", m_transform.position,0.0f,6.0f);
	Destroy();
}