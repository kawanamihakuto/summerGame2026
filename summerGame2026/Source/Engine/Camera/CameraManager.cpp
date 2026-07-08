#include "CameraManager.h"
#include"FollowCamera.h"
CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::Init()
{
	for (auto& c : m_camrea)
	{
		c->Init();
	}
}

void CameraManager::Update()
{
	for (auto& c : m_camrea)
	{
		c->Update();
	}
}

void CameraManager::AddCamera(std::shared_ptr<CameraBase> camera)
{
	m_camrea.push_back(camera);
}

void CameraManager::Apply()
{
	for (auto& c : m_camrea)
	{
		if (c->GetIsApply())
		{
			c->Apply();
		}
	}
}

Transform& CameraManager::GetTransfrom()
{
	for (auto& c : m_camrea)
	{
		if (c->GetIsApply())
		{
			return c->GetTransform();
		}
	}
}

const Transform& CameraManager::GetTransfrom() const
{
	for (auto& c : m_camrea)
	{
		if (c->GetIsApply())
		{
			return c->GetTransform();
		}
	}
}

void CameraManager::ChangeCamera(CameraName name)
{
	for (auto& c : m_camrea)
	{
		if (c->GetCameraName() == name)
		{
			c->SetIsApply(true);
		}
		else
		{
			c->SetIsApply(false);
		}
	}
}

FollowCamera& CameraManager::GetFollowCamera()const
{
	for (auto& c : m_camrea)
	{
		if (auto cast = dynamic_cast<FollowCamera*>(c.get()))
		{
			return *cast;
		}
	}
}
