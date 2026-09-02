#include "GameScene.h"
#include"SceneController.h"
#include"Engine/Core/Application.h"
#include"ClearScene.h"
#include"Engine/Math/Vector3.h"
#include"Engine/Core/ResourceManager.h"
#include"Engine/Core/GameObjectManager.h"
#include"Engine/Core/EffectManager.h"
#include"Engine/Input/PlayerController.h"
#include"Engine/Input/InputManager.h"
#include"Engine/Capture/CaptureManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
#include"Game/Stage/Stage.h"
#include"Game/Player/Player.h"
#include"Game/Item/Hat.h"
#include"Game/Graphics/SkyBox.h"
#include"Game/Enemy/CrabEnemy.h"
#include"Game/Item/Star.h"
#include"Game/Enemy/EnemySpawner.h"
#include"Game/Graphics/UI.h"
#include"Engine/Core/SoundManager.h"
namespace
{
	//フェードにかかる時間
	constexpr int kFadeInterval = 60;

	//カメラ設定
	constexpr Vector3 kCameraPosition = { 0.0f, 300.0f, -1000.0f };
	constexpr Vector3 kCameraTarget = { 0.0f, 0.0f, 0.0f };
	constexpr float kFov = DX_PI_F / 3.0f;
	constexpr float kCameraNear = 50.0f;
	constexpr float kCameraFar = 10000.0f;

	constexpr int kStarNum = 5;

	constexpr int kPlayerMaxHp = 5;

	constexpr Vector3 kStarPos[kStarNum] = {
		{-1700, -350.0f, -200.0f } ,
		{-500,-350,1540},
		{2735,-150,3960},
		{3308,1000,-1884},
		{4967,300,970}
	};

	constexpr int kSpawnerNum = 4;

	constexpr Vector3 kSpawnpos[kSpawnerNum] = {
		{-2830, -1116, -230},
		{462,-516,-277},
		{5000,-1016,-416},
		{2813,-516,-277}
	};
}

GameScene::GameScene(SceneController& controller) :
	SceneBase(controller),
	m_frameCount(0),
	m_isReset(false)
{
	//フェードイン
	m_update = &GameScene::FadeInUpdate;
	//フェード用描画
	m_draw = &GameScene::FadeDraw;
	//フェード用のフレームカウンター初期化
	m_fadeFrame = kFadeInterval;

	// Zバッファの設定
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	// カメラの設定
//	SetCameraPositionAndTarget_UpVecY(kCameraPosition, kCameraTarget);
	SetupCamera_Perspective(kFov);
	SetCameraNearFar(kCameraNear, kCameraFar);

	auto& resouceManager = ResourceManager::GetInstance();

		//コリジョンマネージャー生成
	m_collisionManager = std::make_shared<CollisionManager>();
	//カメラマネージャー生成
	m_cameraManager = std::make_shared<CameraManager>();
	//ゲームオブジェクトマネージャー生成
	m_gameObjectManager = std::make_shared<GameObjectManager>(*m_collisionManager);
	//プレイヤー生成
	m_gameObjectManager->Add(std::make_unique<Player>(resouceManager.GetModel(ModelType::player), resouceManager.GetModel(ModelType::stageCollider), *m_cameraManager, kPlayerMaxHp));
	//帽子生成
	m_gameObjectManager->Add(std::make_unique<Hat>(resouceManager.GetModel(ModelType::hat), resouceManager.GetModel(ModelType::stageCollider), m_gameObjectManager->Find<Player>()));
	//スター生成
	for (int i = 0; i < kStarNum; i++)
	{
		m_gameObjectManager->Add(std::make_unique<Star>(resouceManager.GetModel(ModelType::star), kStarPos[i]));
	}
	//フォローカメラ追加
	m_cameraManager->AddCamera(std::make_shared<FollowCamera>(m_gameObjectManager->Find<Player>()));
	//現在のカメラをセット
	m_cameraManager->ChangeCamera(CameraName::follow);
	//ステージ
	m_stage = std::make_shared<Stage>(resouceManager.GetModel(ModelType::stageCollider), resouceManager.GetModel(ModelType::stageAppearance));
	//ステージとの当たり判定をするためのセットアップ
	MV1SetupCollInfo(resouceManager.GetModel(ModelType::stageCollider), -1, 8, 8, 8);
	//キャプチャーマネージャー生成
	m_captureManager = std::make_shared<CaptureManager>(
		m_gameObjectManager->Find<Player>(),
		m_gameObjectManager->Find<Hat>(),
		*m_cameraManager,
		m_gameObjectManager->Find<Player>());
	//敵スポナー生成
	for (int i = 0; i < kSpawnerNum; i++)
	{
		m_enemySpawners.push_back(std::make_shared<EnemySpawner>(*m_gameObjectManager, resouceManager.GetModel(ModelType::crabEnemy),
			resouceManager.GetModel(ModelType::stageCollider), *m_cameraManager, *m_captureManager, kSpawnpos[i]));
	}

	//ゲームオブジェクト全体を初期化
	m_gameObjectManager->Init();
	//プレイヤーコントローラー生成
	m_playerController = std::make_shared<PlayerController>(m_gameObjectManager->Find<Hat>(), m_gameObjectManager->Find<Player>(), *m_captureManager);
	//スカイボックス生成
	m_skyBox = std::make_shared<SkyBox>(
		resouceManager.GetGraph(GraphType::skyFront),
		resouceManager.GetGraph(GraphType::skyRight),
		resouceManager.GetGraph(GraphType::skyBack),
		resouceManager.GetGraph(GraphType::skyLeft),
		resouceManager.GetGraph(GraphType::skyUp),
		resouceManager.GetGraph(GraphType::skyBottom)
	);
	//カメラポジションをセット
	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());

	//エフェクトマネージャー初期化
	EffectManager::Init();
	//エフェクトのロード
	EffectManager::Load("waitSpawn", ResourceManager::GetInstance().GetEffectPath(EffectType::waitSpawn));
	EffectManager::Load("spawn", ResourceManager::GetInstance().GetEffectPath(EffectType::spawn));
	EffectManager::Load("jump", ResourceManager::GetInstance().GetEffectPath(EffectType::jump));
	EffectManager::Load("getStar", ResourceManager::GetInstance().GetEffectPath(EffectType::getStar));
	EffectManager::Load("enemyDeath", ResourceManager::GetInstance().GetEffectPath(EffectType::enemyDeath));
	//UI生成
	m_UI = std::make_shared<UI>(resouceManager.GetGraph(GraphType::UI), kStarNum, kPlayerMaxHp);

	SoundManager::PlayBGM("game");
}

GameScene::~GameScene()
{
	//ゲームオブジェクトクリア
	m_gameObjectManager->Clear();
	//当たり判定オブジェクトクリア
	m_collisionManager->ClearObjects();

	EffectManager::StopAll();
}

void GameScene::Update()
{
	(this->*m_update)();
}

void GameScene::Draw()
{
	(this->*m_draw)();
}

void GameScene::FadeInUpdate()
{
	if (--m_fadeFrame <= 0)
	{
		m_update = &GameScene::NormalUpdate;
		m_draw = &GameScene::NormalDraw;
		return;
	}
}

void GameScene::NormalUpdate()
{
	m_frameCount++;

	//入力取得用
	auto& input = InputManager::GetInstance();
	input.Update();

	//プレイヤーコントローラー更新
	m_playerController->Update();
	//ゲームオブジェクト更新
	m_gameObjectManager->Update();

	//敵生成更新
	for (auto& spawner : m_enemySpawners)
	{
		spawner->Update();
	}

	//全体の当たり判定
	m_collisionManager->CheckAllCollisions();
	//カメラ更新
	m_cameraManager->Update();
	//DxLibに反映
	m_cameraManager->Apply();
	//スカイボックスにカメラポジションセット
	m_skyBox->SetCameraPos(m_cameraManager->GetTransfrom().GetPosition());

	//エフェクト更新
	EffectManager::Update();

#ifdef _DEBUG
	
#endif // _DEBUG

	if (input.IsTriggered("START"))
	{
		m_update = &GameScene::FadeOutUpdate;
		m_draw = &GameScene::FadeDraw;
		SoundManager::FadeOutBGM(kFadeInterval);
	}

	if (!m_gameObjectManager->Find<Star>())
	{
		m_update = &GameScene::FadeOutUpdate;
		m_draw = &GameScene::FadeDraw;
		SoundManager::FadeOutBGM(kFadeInterval);
	}

	if (m_gameObjectManager->Find<Player>()->GetHp() <= 0)
	{
		m_isReset = true;
		m_update = &GameScene::FadeOutUpdate;
		m_draw = &GameScene::FadeDraw;
		SoundManager::FadeOutBGM(kFadeInterval);
	}
}

void GameScene::FadeOutUpdate()
{
	if (m_fadeFrame++ >= kFadeInterval)
	{
		if (m_isReset)
		{
			//ゲームシーンリセット
			m_controller.ChangeScene(std::make_shared<GameScene>(m_controller));
			m_isReset = false;
			return;
		}
		else
		{
			//シーンの切り替え
			m_controller.ChangeScene(std::make_shared<ClearScene>(m_controller));
			return;
		}
	}
}

void GameScene::NormalDraw()
{
	//スカイボックス描画
	m_skyBox->Draw();
	//ステージ描画
	m_stage->Draw();
	//ゲームオブジェクト描画
	m_gameObjectManager->Draw();
	//敵生成描画
	for (const auto& spawner : m_enemySpawners)
	{
		spawner->Draw();
	}

	//エフェクト描画
	EffectManager::Draw();

	//UI描画
	m_UI->Draw(m_gameObjectManager->GetStarCount(), m_gameObjectManager->Find<Player>()->GetHp());

	DrawFormatString(0, 48, 0x0000ff, L"STARS:%d", m_gameObjectManager->GetStarCount());

	DrawFormatString(0, 64, 0x0000ff, L"PLAYER HP:%d", m_gameObjectManager->Find<Player>()->GetHp());

#ifdef _DEBUG
	DrawFormatString(0, 16, 0x0000ff, L"FRAME:%d", m_frameCount);
	float fps = GetFPS();
	DrawFormatString(0, 32, 0x0000ff, L"FPS:%f", fps);

#endif // _DEBUG
}

void GameScene::FadeDraw()
{
	NormalDraw();

	auto& wsize = Application::GetInstance().GetWindowSize();
	//フェード処理
	float rate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeInterval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * rate));
	if (m_update == &GameScene::FadeInUpdate)
	{
		DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	}
	else if (m_update == &GameScene::FadeOutUpdate)
	{
		if (m_isReset)
		{
			DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
		}
		else
		{
			DrawBox(0, 0, wsize.w, wsize.h, 0xffffff, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, static_cast<int>(255 * rate));
}
