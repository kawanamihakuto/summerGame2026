#include "EnemySpawner.h"
#include"Engine/Core/GameObjectManager.h"
#include"CrabEnemy.h"
#include"Engine/Core/EffectManager.h"

namespace
{
	//スポーン判定範囲
	constexpr float kSpawnRadius = 100.0f;
	//スポーンしない範囲
	constexpr float kNotSpawnRadius = 30.0f;
	//最大同時スポーン数
	constexpr int kMaxEnemyCount = 7;
	//スポーンチェック間隔
	constexpr int kSpawnCheckInterval = 300;
	//スポーン間隔
	constexpr int kSpawnInterval = 130;
}

EnemySpawner::EnemySpawner(GameObjectManager& gameObjectManager, int enemyModel, int stageModel, CameraManager& cameraManager, CaptureManager& captureManager, const Vector3& spawnPosition):
	m_gameObjectManager(gameObjectManager),
	m_captureManager(captureManager),
	m_cameraManager(cameraManager),
	m_enemyModel(enemyModel),
	m_stageModel(stageModel),
	m_frameCount(0),
	m_spawnIntervalCount(0)
{
	m_transform.SetPosition(spawnPosition);
}

void EnemySpawner::Init()
{
	
}

void EnemySpawner::End()
{
}

void EnemySpawner::Update()
{
	m_frameCount++;

	if(m_frameCount >= kSpawnCheckInterval)
	{
		m_frameCount = 0;
		if (m_gameObjectManager.GetEnemyCountNear(m_transform.position, kSpawnRadius) < kMaxEnemyCount)
		{
			if (m_gameObjectManager.GetEnemyCountNear(m_transform.position, kNotSpawnRadius) == 0)
			{
				m_isNextSpawn = true;
				m_spawnIntervalCount = 0;
				EffectManager::Play("waitSpawn", m_transform.position, 0.0f);
			}
		}
	}

	if(m_isNextSpawn)
	{
		m_spawnIntervalCount++;
		if(m_spawnIntervalCount >= kSpawnInterval)
		{
			m_isNextSpawn = false;
			Spawn();
			EffectManager::Play("spawn", m_transform.position, 0.0f);
		}
	}
}

void EnemySpawner::Draw()
{
	
}

void EnemySpawner::Spawn()
{
	m_gameObjectManager.Add(std::make_unique<CrabEnemy>(m_enemyModel, m_stageModel, m_cameraManager, m_captureManager, m_transform.position));
}
