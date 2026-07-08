#pragma once
#include"CameraBase.h"
#include"Engine/Core/Precompiled.h"

class FollowCamera;
class CameraBase;
class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	void Init();
	void Update();
	void AddCamera(std::shared_ptr<CameraBase> camera);
	void Apply();

	Transform& GetTransfrom();
	const Transform& GetTransfrom()const;

	void ChangeCamera(CameraName name);

	FollowCamera& GetFollowCamera()const;

private:
	std::vector<std::shared_ptr<CameraBase>> m_camrea;
};
