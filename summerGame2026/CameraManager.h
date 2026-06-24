#pragma once
class CameraManager
{
private:
	CameraManager();
	// コピー禁止
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;
public:
	void Init();
	void Update();
};

