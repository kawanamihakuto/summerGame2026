#pragma once
#include"Engine/Core/Precompiled.h"
#include"Engine/Core/GameObjectManager.h"
#include"Engine/Collision/CollisionManager.h"

class CameraManager;
class Stage;
class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void Update();
	void Draw();
	void End();
private:
	void DrawGrid();

private:
	int m_frameCount;

	std::shared_ptr<Stage>m_stage;

	std::shared_ptr<GameObjectManager> m_gameObjectManager;

	std::shared_ptr<CollisionManager> m_collisionManager;

	std::shared_ptr<CameraManager> m_cameraManager;
};

