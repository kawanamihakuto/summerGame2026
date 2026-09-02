#include "UI.h"
#include"Engine/Math/Vector2.h"
#include<DxLib.h>
#include"Engine/Core/Application.h"
namespace
{
	constexpr int kSrcWidth = 16;
	constexpr int kSrcHeight = 16;

	constexpr Vector2 kHeartSrcPos = { 0, 0 };
	constexpr Vector2 kEmptyHeartSrcPos = { 16, 16 };

	constexpr Vector2 kStarSrcPos = { 0, 64 };
	constexpr Vector2 kEmptyStarSrcPos = { 16, 64 };

	constexpr int kDrawWidth = 96;
	constexpr int kDrawHeight = 96;
}

UI::UI(int UIGraphHandle, int maxStarNum, int maxHP):
	m_UIGraphHandle(UIGraphHandle),
	m_maxStarNum(maxStarNum),
	m_maxHP(maxHP)
{
}

UI::~UI()
{
}

void UI::Update()
{
}

void UI::Draw(int starNum, int hp)
{
	auto& wsize = Application::GetInstance().GetWindowSize();

	for (int i = 0; i < m_maxStarNum; i++)
	{
		if (i < m_maxStarNum - starNum)
		{
			//星
			DrawRectExtendGraph(kDrawWidth * i,0,kDrawWidth * (i + 1),
				kDrawHeight,kStarSrcPos.x, kStarSrcPos.y,
				kSrcWidth, kSrcHeight + 1,m_UIGraphHandle,true);
		}
		else
		{
			//空の星
			DrawRectExtendGraph(kDrawWidth * i, 0, kDrawWidth * (i + 1),
				kDrawHeight, kEmptyStarSrcPos.x + 1, kEmptyStarSrcPos.y,
				kSrcWidth + 1, kSrcHeight + 1, m_UIGraphHandle, true);
		}
	}



	for (int i = 0; i < m_maxHP; i++)
	{
		if (i < m_maxHP - hp)
		{
			//空のハート
			DrawRectExtendGraph(wsize.w - kDrawWidth * i, 0, wsize.w - kDrawWidth * (i + 1),
				kDrawHeight, kEmptyHeartSrcPos.x, kEmptyHeartSrcPos.y - 1 ,
				kSrcWidth, kSrcHeight, m_UIGraphHandle, true);
		}
		else
		{
			//ハート
			DrawRectExtendGraph(wsize.w - kDrawWidth * i, 0, wsize.w - kDrawWidth * (i + 1),
				kDrawHeight, kHeartSrcPos.x, kHeartSrcPos.y,
				kSrcWidth, kSrcHeight, m_UIGraphHandle, true);
		}
	}

}
