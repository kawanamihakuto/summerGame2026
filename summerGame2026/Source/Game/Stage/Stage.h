#pragma once
class Stage
{
public:
	Stage(int stageColliderModel,int stageAppearanceModel);
	~Stage();
	void Draw();
private:
	int m_colliderModelHandle;
	int m_appearanceModelHandle;
};

