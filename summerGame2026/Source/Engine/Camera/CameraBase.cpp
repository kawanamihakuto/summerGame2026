#include "CameraBase.h"
#include"CameraManager.h"

void CameraBase::SetIsApply(bool is)
{
	m_isApply = is;
}

bool CameraBase::GetIsApply()
{
	return m_isApply;
}

Transform& CameraBase::GetTransform()
{
	return m_transform;
}

const Transform& CameraBase::GetTransform() const
{
	return m_transform;
}
