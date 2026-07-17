#pragma once
class Stage
{
public:
	Stage(int stageModel);
	~Stage();
	void Draw();
private:
	int m_modelHandle;
};

