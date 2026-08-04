#include "ClearScene.h"
#include"SceneController.h"
#include"TitleScene.h"
#include"Engine/Core/Application.h"
#include"Engine/Input/InputManager.h"

namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;
}

ClearScene::ClearScene(SceneController& controller):
	SceneBase(controller)
{
	//フェードイン
	m_update = &ClearScene::FadeInUpdate;
	//フェード用描画
	m_draw = &ClearScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_frame = kFadeInterval;
}

ClearScene::~ClearScene()
{
}

void ClearScene::Update()
{
	(this->*m_update)();
}

void ClearScene::Draw()
{
	(this->*m_draw)();
}

void ClearScene::FadeInUpdate()
{
	if (--m_frame <= 0)
	{
		m_update = &ClearScene::NormalUpdate;
		m_draw = &ClearScene::NormalDraw;
		return;
	}
}

void ClearScene::NormalUpdate()
{
	auto& input = InputManager::GetInstance();
	input.Update();

	if (input.AnyPressButtonInput())
	{
		m_update = &ClearScene::FadeOutUpdate;
		m_draw = &ClearScene::FadeDraw;
		m_frame = 0;
	}
}

void ClearScene::FadeOutUpdate()
{
	if (++m_frame >= kFadeInterval)
	{
		//シーンの切り替え
		m_controller.ChangeScene(std::make_shared<TitleScene>(m_controller));
		return;
	}
}

void ClearScene::NormalDraw()
{
	auto& wsize = Application::GetInstance().GetWindowSize();

	DrawFormatString(wsize.w / 2.0f, wsize.h / 2.0f, 0xffffff, L"ClearScene");
}

void ClearScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}
