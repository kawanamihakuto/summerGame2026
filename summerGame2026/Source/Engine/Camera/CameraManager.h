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
	//カメラの追加
	void AddCamera(std::shared_ptr<CameraBase> camera);
	//DxLibに反映
	void Apply();
	//DxLibに反映するカメラの変更
	void ChangeCamera(CameraName name);

	//トランスフォーム取得
	Transform& GetTransfrom();
	const Transform& GetTransfrom()const;
	//フォローカメラ取得
	FollowCamera& GetFollowCamera()const;

private:
	//カメラたち
	std::vector<std::shared_ptr<CameraBase>> m_camrea;
};
