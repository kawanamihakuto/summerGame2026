#pragma once
#include"CameraBase.h"
#include"Engine/Core/Precompiled.h"
class CameraManager
{
private:
	CameraManager() = default;
	// コピー禁止
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;
public:

	static CameraManager& GetInstance();

	void Init(std::unique_ptr<CameraBase>camera);
	void Update();
	void Apply();

	void ChangeCamera(std::unique_ptr<CameraBase> newCamera);

	Transform& GetTransfrom();
	const Transform& GetTransfrom()const;

private:
	std::unique_ptr<CameraBase>m_currentCamera;
};

