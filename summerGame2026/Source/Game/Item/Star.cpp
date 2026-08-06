#include "Star.h"
#include"Engine/Collision/SphereCollider.h"
namespace
{
	constexpr float kSphereRadius = 60.0f;
}

Star::Star(int modelHandle, const Vector3& pos)
{
	m_modelHandle = MV1DuplicateModel(modelHandle);
	m_transform.SetPosition(pos);
	m_bodyCollider = std::make_unique<SphereCollider>(m_transform.position, kSphereRadius);
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
	Destroy();
}