#include "SkyBox.h"
#include"DxLib.h"

namespace
{
	constexpr float kSize = 1500.0f;
}

SkyBox::SkyBox(int front, int right, int back, int left, int up, int bottom) :
	m_cameraPos(0.0f, 0.0f, 0.0f),
	m_frontHandle(front),
	m_rightHandle(right),
	m_backHandle(back),
	m_leftHandle(left),
	m_upHandle(up),
	m_bottomHandle(bottom)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::SetCameraPos(const Vector3& cameraPos)
{
	m_cameraPos = cameraPos;
}

void SkyBox::Draw()
{
	VECTOR cameraPos = VGet(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);

	SetUseLighting(false);
	SetUseBackCulling(false);
	SetWriteZBufferFlag(false);
	//前、右、後、左、上、下の順番で描画
	DrawSkyBox(VAdd(cameraPos, VGet(-kSize, kSize, -kSize)), VAdd(cameraPos, VGet(-kSize, -kSize, -kSize)), VAdd(cameraPos, VGet(kSize, -kSize, -kSize)), VAdd(cameraPos, VGet(kSize, kSize, -kSize)), m_frontHandle);
	DrawSkyBox(VAdd(cameraPos, VGet(kSize, kSize, -kSize)), VAdd(cameraPos, VGet(kSize, -kSize, -kSize)), VAdd(cameraPos, VGet(kSize, -kSize, kSize)), VAdd(cameraPos, VGet(kSize, kSize, kSize)), m_rightHandle);
	DrawSkyBox(VAdd(cameraPos, VGet(kSize, kSize, kSize)), VAdd(cameraPos, VGet(kSize, -kSize, kSize)), VAdd(cameraPos, VGet(-kSize, -kSize, kSize)), VAdd(cameraPos, VGet(-kSize, kSize, kSize)), m_backHandle);
	DrawSkyBox(VAdd(cameraPos, VGet(-kSize, kSize, kSize)), VAdd(cameraPos, VGet(-kSize, -kSize, kSize)), VAdd(cameraPos, VGet(-kSize, -kSize, -kSize)), VAdd(cameraPos, VGet(-kSize, kSize, -kSize)), m_leftHandle);
	DrawSkyBox(VAdd(cameraPos, VGet(kSize, kSize, kSize)), VAdd(cameraPos, VGet(-kSize, kSize, kSize)), VAdd(cameraPos, VGet(-kSize, kSize, -kSize)), VAdd(cameraPos, VGet(kSize, kSize, -kSize)), m_upHandle);
	DrawSkyBox(VAdd(cameraPos, VGet(-kSize, -kSize, -kSize)), VAdd(cameraPos, VGet(-kSize, -kSize, kSize)), VAdd(cameraPos, VGet(kSize, -kSize, kSize)), VAdd(cameraPos, VGet(kSize, -kSize, -kSize)), m_bottomHandle);

	SetWriteZBufferFlag(true);
	SetUseBackCulling(true);
	SetUseLighting(true);
}

void SkyBox::DrawSkyBox(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, int handle)
{
	VERTEX3D vertex[4];
	WORD index[6];

	vertex[0].pos = p0;
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;

	vertex[1].pos = p1;
	vertex[1].u = 0.0f;
	vertex[1].v = 1.0f;

	vertex[2].pos = p2;
	vertex[2].u = 1.0f;
	vertex[2].v = 1.0f;

	vertex[3].pos = p3;
	vertex[3].u = 1.0f;
	vertex[3].v = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		vertex[i].norm = VGet(0.0f, 0.0f, 1.0f);
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
	}

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	SetWriteZBuffer3D(false);
	DrawPolygonIndexed3D(vertex, 4, index, 2, handle, false);
	SetWriteZBuffer3D(true);
}