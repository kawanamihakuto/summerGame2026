#pragma once
#include"Engine/Core/GameObject.h"

class CaptureManager;
class CameraManager;
class GameObjectManager;
class EnemySpawner : public GameObject
{
public:
	EnemySpawner(GameObjectManager& gameObjectManager,int enemyModel,int stageModel,CameraManager& cameraManager,CaptureManager& captureManager,const Vector3& spawnPosition);

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	void Spawn();

private:
	int m_frameCount;
	int m_spawnIntervalCount;

	bool m_isNextSpawn = false;

	GameObjectManager& m_gameObjectManager;
	CaptureManager& m_captureManager;
	CameraManager& m_cameraManager;
	int m_enemyModel;
	int m_stageModel;
};

