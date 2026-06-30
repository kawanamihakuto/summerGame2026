#pragma once
#include"Engine/Core/Precompiled.h"
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
};

