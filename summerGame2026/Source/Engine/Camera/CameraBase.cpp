#include "CameraBase.h"

Transform& CameraBase::GetTransform()
{
	return m_transform;
}

const Transform& CameraBase::GetTransform() const
{
	return m_transform;
}
