#pragma once
#include"Engine/Math/Vector3.h"
#include"DxLib.h"
class SkyBox
{
public:
	SkyBox(int front,int right,int back,int left,int up,int bottom);
	~SkyBox();
	void SetCameraPos(const Vector3& cameraPos);
	void Draw();

	void DrawSkyBox(VECTOR p0, VECTOR p1, VECTOR p2, VECTOR p3, int handle);
private:
	Vector3 m_cameraPos;

	int m_frontHandle;
	int m_leftHandle;
	int m_rightHandle;
	int m_backHandle;
	int m_upHandle;
	int m_bottomHandle;
};
