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
	//全部初期化
	for (auto& c : m_camrea)
	{
		c->Init();
	}
}

void CameraManager::Update()
{
	//全部更新
	for (auto& c : m_camrea)
	{
		c->Update();
	}
}

void CameraManager::AddCamera(std::shared_ptr<CameraBase> camera)
{
	//追加
	m_camrea.push_back(camera);
}

void CameraManager::Apply()
{
	//フラグが立ってるやつを反映
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
	//トランスフォーム取得
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
	//トランスフォーム取得
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
	//nameから反映するカメラを探す
	//それ以外は反映しないようにする
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
	//フォローカメラ取得
	for (auto& c : m_camrea)
	{
		if (auto cast = dynamic_cast<FollowCamera*>(c.get()))
		{
			return *cast;
		}
	}
}
