#pragma once
#include"Engine/Input/PlayerController.h"
#include"Engine/Camera/CameraManager.h"
#include"ICaptureTarget.h"

/// <summary>
/// キャプチャー対象を保存して管理するクラス
/// </summary>
class CaptureManager
{
public:
	/// <summary>
	/// コンストラクタでもろもろもらうよ
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="hat">帽子</param>
	/// <param name="camreaManager">カメラマネージャー</param>
	/// <param name="target">ターゲット</param>
	CaptureManager(Player* player,Hat* hat,CameraManager& camreaManager,ICaptureTarget* target);

	//キャプチャー
	void Capture(ICaptureTarget* target);
	//キャプチャーの解除
	void Release();
	//現在のターゲット取得
	ICaptureTarget* GetTarget();
private:
	Player* m_player;
	Hat* m_hat;
	CameraManager& m_cameraManager;
	ICaptureTarget* m_currentTarget = nullptr;
};

