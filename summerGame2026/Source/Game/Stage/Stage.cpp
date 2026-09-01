#include "Stage.h"
#include<DxLib.h>
#include"Engine/Math/Vector3.h"
Stage::Stage(int stageColliderModel,int stageAppearanceModel):
	m_colliderModelHandle(-1),
	m_appearanceModelHandle(-1)
{
	m_colliderModelHandle = stageColliderModel;
	MV1SetPosition(m_colliderModelHandle, Vector3::Zero());
	m_appearanceModelHandle = stageAppearanceModel;
	MV1SetPosition(m_appearanceModelHandle, Vector3::Zero());
}

Stage::~Stage()
{
}

void Stage::Draw()
{
#ifdef _DEBUG
	MV1DrawModel(m_colliderModelHandle);
#else
	MV1DrawModel(m_appearanceModelHandle);
#endif // _DEBUG
}
