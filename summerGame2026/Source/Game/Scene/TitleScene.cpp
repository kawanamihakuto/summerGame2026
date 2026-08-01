#include "TitleScene.h"

TitleScene::TitleScene(SceneController& controller):
	SceneBase(controller)
{
	//フェードイン
	m_update = &TitleScene::FadeInUpdate;
	//フェード用描画
	m_draw = &TitleScene::FadeDraw;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
}

void TitleScene::FadeInUpdate()
{
}

void TitleScene::NormalUpdate()
{
}

void TitleScene::FadeOutUpdate()
{
}

void TitleScene::NormalDraw()
{
}

void TitleScene::FadeDraw()
{
}
