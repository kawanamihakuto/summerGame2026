#pragma once
#include "SceneBase.h"
#include "Engine/Core/PreCompiled.h"

class SkyBox;
class CameraManager;
class GameObjectManager;
class CollisionManager;
class GameOverScene : public SceneBase
{
public:
	GameOverScene(SceneController& controller);
	~GameOverScene();
	void Update()override;
	void Draw()override;

private:
	//フェードイン更新
	void FadeInUpdate();
	//通常更新
	void NormalUpdate();
	//フェードアウト更新
	void FadeOutUpdate();
	//Updateメンバ関数を代入できるメンバ関数ポインタ
	using UpdateFunc_t = void (GameOverScene::*)();
	UpdateFunc_t m_update;

	//通常Draw
	void NormalDraw();
	//フェードDraw
	void FadeDraw();
	//Drawメンバ関数を代入できるメンバ関数ポインタ
	using DrawFunc_t = void(GameOverScene::*)();
	DrawFunc_t m_draw;

	//フェード用のフレームカウンター
	int m_frame;

	//ゲームオブジェクトを管理する
	std::shared_ptr<GameObjectManager> m_gameObjectManager;
	//当たり判定をする
	std::shared_ptr<CollisionManager> m_collisionManager;
	//カメラを管理する
	std::shared_ptr<CameraManager> m_cameraManager;
	//スカイボックス
	std::shared_ptr<SkyBox>m_skyBox;
};

