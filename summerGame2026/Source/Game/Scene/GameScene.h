#pragma once
#include "SceneBase.h"
#include"Engine/Core/Precompiled.h"

class Stage;
class GameObjectManager;
class CollisionManager;
class CameraManager;
class PlayerController;
class CaptureManager;
class SkyBox;
class GameScene : public SceneBase
{
public:
	GameScene(SceneController& controller);
	~GameScene();
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
	using UpdateFunc_t = void (GameScene::*)();
	UpdateFunc_t m_update;

	//通常Draw
	void NormalDraw();
	//フェードDraw
	void FadeDraw();
	//Drawメンバ関数を代入できるメンバ関数ポインタ
	using DrawFunc_t = void(GameScene::*)();
	DrawFunc_t m_draw;

private:
	int m_fadeFrame;

	int m_frameCount;

	//ステージ
	std::shared_ptr<Stage>m_stage;
	//ゲームオブジェクトを管理する
	std::shared_ptr<GameObjectManager> m_gameObjectManager;
	//当たり判定をする
	std::shared_ptr<CollisionManager> m_collisionManager;
	//カメラを管理する
	std::shared_ptr<CameraManager> m_cameraManager;
	//現在の操作対象を保存して入力を適用する
	std::shared_ptr<PlayerController> m_playerController;
	//キャプチャー対象を保存し、通知する
	std::shared_ptr<CaptureManager> m_captureManager;
	//スカイボックス
	std::shared_ptr<SkyBox>m_skyBox;
};

