#include "CameraManager.h"

CameraManager& CameraManager::GetInstance()
{
	static CameraManager instance;
	return instance;
}

void CameraManager::Init(std::unique_ptr<CameraBase>camera)
{
	m_currentCamera = std::move(camera);
	m_currentCamera->Init();
}

void CameraManager::Update()
{
	m_currentCamera->Update();
}

void CameraManager::Apply()
{
	m_currentCamera->Apply();
}

void CameraManager::ChangeCamera(std::unique_ptr<CameraBase> newCamera)
{
	if (m_currentCamera != newCamera)
	{
		m_currentCamera->End();

		m_currentCamera = std::move(newCamera);

		m_currentCamera->Init();
	}
}

Transform& CameraManager::GetTransfrom()
{
	return m_currentCamera->GetTransform();
}

const Transform& CameraManager::GetTransfrom() const
{
	return m_currentCamera->GetTransform();
}
