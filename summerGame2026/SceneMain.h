#pragma once
#include"Engine/Core/Precompiled.h"

class Stage;
class GameObjectManager;
class CollisionManager;
class CameraManager;
class PlayerController;
class CaptureManager;
class SkyBox;

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void Update();
	void Draw();
	void End();

private:
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

	bool m_isEnemyGenerate;
};

