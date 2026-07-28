#include "PlayerController.h"
#include"InputManager.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Camera/FollowCamera.h"
#include"Engine/Capture/CaptureManager.h"
PlayerController::PlayerController(Hat* hat, Player* player, CaptureManager& captureManager):
	m_hat(hat),
	m_player(player),
	m_captureManager(captureManager),
	m_target(player)
{
	
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	//入力取得
	auto& input = InputManager::GetInstance();
	Vector2 move = input.GetLeftStick();
	//操作対象のオブジェクトをターゲットにする
	m_target = m_target->GetControllTarget();
	//移動
	if (m_target)
	{
		m_target->Move(move);
	}
	//ジャンプ開始
	if (input.IsTriggered("A"))
	{
		m_target->Jump();
	}
	//ジャンプボタンを離したら上昇を切る
	if (input.IsReleased("A"))
	{
		m_target->JumpEnd();
	}

	//帽子を投げる
	if (input.IsTriggered("X"))
	{
		//プレイヤーがアクティブかどうか
		if (m_player->IsActive())
		{
			//投げる
			m_hat->Throw(m_player->GetLastMoveDirection());
		}
	}
	//キャプチャー解除
	if (input.IsTriggered("Y"))
	{
		//プレイヤーがアクティブじゃないかどうか
		if (!m_player->IsActive())
		{
			//キャプチャー解除
			m_captureManager.Release();
			//操作対象をプレイヤーに戻す
			SetTarget(m_player);
		}
	}

	//帽子からキャプチャーしたかを取得
	if (m_hat->GetCaptureFlag())
	{
		//キャプチャーマネージャーに通知
		m_captureManager.Capture(m_hat->GetCaptureTarget());
		SetTarget(m_hat->GetCaptureTarget());
		m_hat->ResetCaptureFlag();
	}
}

void PlayerController::SetTarget(ICaptureTarget* target)
{
	if (m_target)
	{
		//前のターゲットのExitを呼ぶ
		m_target->ExitControll();
	}
	//次のターゲットをセット
	m_target = target->GetControllTarget();
	m_target->Controll();
}