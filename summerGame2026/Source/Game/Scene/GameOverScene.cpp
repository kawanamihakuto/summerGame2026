#include "GameOverScene.h"

GameOverScene::GameOverScene(SceneController& controller):
	SceneBase(controller)
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Update()
{
}

void GameOverScene::Draw()
{
}

void GameOverScene::FadeInUpdate()
{
}

void GameOverScene::NormalUpdate()
{
}

void GameOverScene::FadeOutUpdate()
{
}

void GameOverScene::NormalDraw()
{
}

void GameOverScene::FadeDraw()
{
	NormalDraw();

//	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
//	float rate = static_cast<float>(m_frame) / static_cast<float>(kFadeInterval);
//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast <int>(255 * rate));
//	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast <int>(255 * rate));
}
