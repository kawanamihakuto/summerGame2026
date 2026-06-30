#include "Stage.h"
#include<DxLib.h>

Stage::Stage(int stageModel):
	m_modelHandle(-1)
{
	m_modelHandle = stageModel;
}

Stage::~Stage()
{
}

void Stage::Draw()
{
	MV1DrawModel(m_modelHandle);
}
