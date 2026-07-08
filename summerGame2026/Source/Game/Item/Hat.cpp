#include "Hat.h"

Hat::Hat(const Vector3& pos, const Vector3& dir)
{
	m_transform.SetPosition(pos);
	m_direction = dir;
}

Hat::~Hat()
{
}

void Hat::Init()
{
}

void Hat::End()
{
}

void Hat::Update()
{
}

void Hat::Draw()
{
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
	return CollisionLayer();
}

CollisionLayer Hat::GetCollisionMask() const
{
	return CollisionLayer();
}

void Hat::OnCollision(ICollider& other)
{
}
