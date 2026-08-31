#include "EnemySpawner.h"
#include"Engine/Core/GameObjectManager.h"
#include"CrabEnemy.h"
namespace
{
	//スポーン判定範囲
	constexpr float kSpawnRadius = 100.0f;
	//最大同時スポーン数
	constexpr int kMaxEnemyCount = 7;
}

EnemySpawner::EnemySpawner(GameObjectManager& gameObjectManager):
	m_gameObjectManager(gameObjectManager)
{
}

void EnemySpawner::Init()
{
}

void EnemySpawner::End()
{
}

void EnemySpawner::Update()
{
	if (m_gameObjectManager.GetEnemyCountNear(m_transform.position, kSpawnRadius) < kMaxEnemyCount)
	{
		Spawn();
	}

}

void EnemySpawner::Draw()
{
}

void EnemySpawner::Spawn()
{
//	m_gameObjectManager.Add(std::make_unique<CrabEnemy>());
}
