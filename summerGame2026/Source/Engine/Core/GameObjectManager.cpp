#include"GameObjectManager.h"
#include"GameObject.h"
GameObjectManager& GameObjectManager::GetInstance()
{
	static GameObjectManager instance;
	return instance;
}
void GameObjectManager::Add(std::unique_ptr<GameObject> object)
{
	//追加
	m_objects.push_back(std::move(object));
}

void GameObjectManager::Init()
{
	//初期化
	for (auto& obj : m_objects)
	{
		obj->Init();
	}
}

void GameObjectManager::Update()
{
	//更新
	for (auto& obj : m_objects)
	{
		obj->Update();
	}
}

void GameObjectManager::Draw()
{
	//描画
	for (auto& obj : m_objects)
	{
		obj->Draw();
	}
}

void GameObjectManager::Clear()
{
	//解放
	m_objects.clear();
}
