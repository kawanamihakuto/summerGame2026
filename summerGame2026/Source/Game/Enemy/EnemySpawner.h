#pragma once
#include"Engine/Core/GameObject.h"

class GameObjectManager;
class EnemySpawner : public GameObject
{
public:
	EnemySpawner(GameObjectManager& gameObjectManager);

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	void Spawn();

private:
	GameObjectManager& m_gameObjectManager;
};

