#pragma once
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
};

