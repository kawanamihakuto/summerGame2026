#pragma once
#include"Engine/Core/PreCompiled.h"
class GameObjectManager;
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

	std::shared_ptr<GameObjectManager>m_gameObjects;
};

