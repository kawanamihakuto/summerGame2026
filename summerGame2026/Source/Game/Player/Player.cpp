#include "Player.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Collision/CollisionManager.h"
#include"Engine/Collision/CapsuleCollider.h"
#include"State/PlayerStateBase.h"
#include"State/PlayerIdleState.h"
#include"State/PlayerJumpState.h"
#include"State/PlayerStunnedState.h"
namespace
{
	//スピード
	constexpr float kWalkSpeed = 4.0f;
	constexpr float kRunSpeed = 12.0f;
	constexpr float kAirMoveSpeedRate = 0.8f;

	//HP
	constexpr int kHp = 3;

	//モデルの回転オフセット
	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());

	//カプセル高さオフセット
	constexpr float kCapsuleHeightOffset = 80.0f;
	//カプセル半径
	constexpr float kCapsuleRadius = 30.0f;
	//カプセル高さ
	constexpr float kCapsuleHeight = 70.0f;
	//レイの数
	constexpr int kGroundRayNum = 4;
	//レイ高さオフセット
	constexpr float kGroundRayHeightOffset = 135.0f;
	//レイ長さ
	constexpr float kGroundRayLength = 140.0f;
	//レイ横幅オフセット
	constexpr float kGroundRayWidthOffset = 15.0f;
	//レイ横オフセットたち
	constexpr Vector3 kGroundRayWidthOffsets[kGroundRayNum] =
	{
		{0.0f                  ,0.0f, kGroundRayWidthOffset  },
		{0.0f                  ,0.0f, -kGroundRayWidthOffset },
		{kGroundRayWidthOffset ,0.0f, 0.0f                   },
		{-kGroundRayWidthOffset,0.0f, 0.0f                   },
	};
	//ジャンプ力
	constexpr float kJumpPower = 20.0f;
	//ジャンプ終了時のvelocityを減らす割合
	constexpr float kJumpEndRate = 0.5f;

	//帽子をかぶせたいフレーム名
	constexpr const wchar_t* kHeadFrameName = L"mixamorig:HeadTop_End";
	//帽子をかぶせたい位置のフレームからのオフセット
	constexpr Vector3 kHeadFrameOffsetMat = { 0.0f,-10.0f,-10.f };

	//走る状態になるスティックの倒し割合
	constexpr float kRunStickRate = 0.7f;

	//踏みつけ判定の法線しきい値
	constexpr float kStompNormalThreshold = -0.5f;

	//無敵時間(フレーム)
	constexpr float kInvincibleFrame = 60.0f * 3.0f;
}

Player::Player(int playerModel, int stageModel, CameraManager& cameraManager) :
	Character(cameraManager),
	m_modelHandle(-1),
	m_stageModelHandle(-1),
	m_isNextJump(false),
	m_isRun(false)
{
	//モデル複製
	m_modelHandle = MV1DuplicateModel(playerModel);
	//ステージとの当たり判定用
	m_stageModelHandle = stageModel;
	//ステージとの当たり判定用コライダー生成
	m_stageCollider = std::make_unique<CapsuleCollider>(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f }, kCapsuleRadius, kCapsuleHeight);

	//ステージとの当たり判定用レイ生成
	m_ray.resize(kGroundRayNum);
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i] = std::make_unique<Ray>(m_transform.position, Vector3{0.0f,-1.0f,0.0f}, kGroundRayLength, kGroundRayWidthOffsets[i], kGroundRayHeightOffset);
	}

	//帽子をかぶせたいフレームインデックス
	m_headFrameIndex = MV1SearchFrame(m_modelHandle, kHeadFrameName);

	//HP初期化
	m_hp = kHp;

	m_isInvincible = false;
	m_InvincibleFrameCount = 0;

	//アニメーションコントローラー
	m_animationController = std::make_shared<AnimationController>(m_modelHandle);
	//アニメーション追加
	m_animationController->AddAnimation(PlayerAnim::idle);
	m_animationController->AddAnimation(PlayerAnim::walk);
	m_animationController->AddAnimation(PlayerAnim::run);
	m_animationController->AddAnimation(PlayerAnim::jump);
	m_animationController->AddAnimation(PlayerAnim::stunned);

	//ステート初期化
	m_state = std::make_unique<PlayerIdleState>(*this, *m_animationController);
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
	
}

void Player::End()
{
}

void Player::Update()
{
	//アクティブだったら
	if (m_isActive)
	{
		//ネクストジャンプフラグが立ってたら
		if (m_isNextJump)
		{
			//ジャンプ
			Jump();
		}

		//ステートの更新
		m_state->Update();

		//速度を決める
		UpdateMove();
		//回転
		UpdateRotate(kModelRotationOffset);
		//重力
		Gravity();

		//ステージとの当たり判定コライダー更新
		m_stageCollider->Update(m_transform.GetPosition() + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f } + m_velocity);
		//ステージ壁との押し戻し
		ResolveWallVelocity(m_stageModelHandle);
		//ステージとの当たり判定レイ更新
		for (auto& ray : m_ray)
		{
			ray->Update(m_transform.GetPosition() + m_velocity);
		}
		//ステージ地面との押し戻し
		GroundCollision(m_stageModelHandle);

		//地面にいないとき
		if (!m_isGround)
		{
			//位置更新
			m_transform.Translate(m_velocity);
		}

		//奈落に落ちたとき用
		ResetPlayerPos({ 0.0f,0.0f,0.0f });

		//モデル更新
		UpdateModel();

		//アニメーション更新
		m_animationController->Update();

		if (m_isInvincible)
		{
			if (m_InvincibleFrameCount++ > kInvincibleFrame)
			{
				m_isInvincible = false;
				m_InvincibleFrameCount = 0;
			}
		}
	}
}

void Player::Draw()
{
	//アクティブだったら
	if (m_isActive)
	{
		if (m_isInvincible && m_InvincibleFrameCount % 10 < 5)
		{
			MV1SetOpacityRate(m_modelHandle, 0.2f);
		}
		//モデル描画
		MV1DrawModel(m_modelHandle);
		
		MV1SetOpacityRate(m_modelHandle, 1.0f);
	}

#ifdef _DEBUG
	//アクティブだったら
	if (m_isActive)
	{
		//当たり判定系描画
		m_stageCollider->Draw();
		for (int i = 0; i < kGroundRayNum; i++)
		{
			m_ray[i]->Draw();
		}
	}
#endif // _DEBUG
}

void Player::ChangeState(std::unique_ptr<PlayerStateBase> newState)
{
	if (m_state != newState)
	{
		m_state->Exit(); 
		
		m_state = std::move(newState);

		m_state->Enter();
	}
}

void Player::StartJamp()
{
	//フラグ
	m_isGround = false;
	m_isNextJump = false;

	m_velocity.y = kJumpPower;
}

void Player::StopMove()
{
	m_moveInput.x = 0.0f;
	m_moveInput.z = 0.0f;
	m_velocity.x = 0.0f;
	m_velocity.z = 0.0f;
}

const Collider& Player::GetCollider() const
{
	return *m_stageCollider;
}

//const Ray& Player::GetRay() const
//{
//	return *m_ray[0];
//}

CameraAnchor Player::GetCameraAnchor() const
{
	return CameraAnchor{ m_transform };
}

CollisionLayer Player::GetCollisionLayer() const
{
	return CollisionLayers::kPlayer;
}

CollisionLayer Player::GetCollisionMask() const
{
	return CollisionLayers::kEnemy | 
		CollisionLayers::kStar;
}

void Player::OnCollision(ICollider& other, CollisionResult& result)
{
	//敵と当たる
	if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
	{
		//踏んでいたら
		if (result.normal.y < kStompNormalThreshold && m_velocity.y < 0.0f)
		{
			//ジャンプフラグ立てる
			m_isNextJump = true;
		}
		else
		{
			if (!m_isInvincible)
			{
				m_hp--;
				m_isInvincible = true;
				ChangeState(std::make_unique<PlayerStunnedState>(*this, *m_animationController));
			}
		}
	}
}

Matrix4x4 Player::GetHatMatrix() const
{
	Matrix4x4 mat = Matrix4x4(MV1GetFrameLocalWorldMatrix(m_modelHandle, m_headFrameIndex));

	Matrix4x4 offsetMat = Matrix4x4::Translate(kHeadFrameOffsetMat);

	mat *= offsetMat;

	return mat;
}

ICaptureTarget* Player::GetControllTarget()
{
	return this;
}

ICaptureTarget* Player::GetHatAndCameraTarget()
{
	return this;
}

GameObject* Player::GetGameObject()
{
	return this;
}

void Player::Move()
{
	auto& camera = m_cameraManager.GetTransfrom();
	//移動ベクトル生成
	m_moveInput = Vector3::Zero();
	m_moveInput += Vector3{ camera.Right().x,0.0f,camera.Right().z } * m_input.move.x;
	m_moveInput += Vector3{ camera.Forward().x,0.0f,camera.Forward().z } * m_input.move.y;
	m_moveInput.Normalize();

	if (fabsf(m_input.move.x) > kRunStickRate ||fabsf(m_input.move.y) > kRunStickRate)
	{
		m_moveInput.x *= kRunSpeed;
		m_moveInput.z *= kRunSpeed;
		m_isRun = true;
	}
	else
	{
		m_moveInput.x *= kWalkSpeed;
		m_moveInput.z *= kWalkSpeed;
		m_isRun = false;
	}

	if (!m_isGround)
	{
		m_moveInput.x *= kAirMoveSpeedRate;
		m_moveInput.z *= kAirMoveSpeedRate;
	}

	m_moveInput.y = 0.0f;

	if (m_moveInput.Length() > 0.0f)
	{
		m_lastMoveInput = m_moveInput;
	}
}

void Player::Jump()
{
	//地面にいるかネクストジャンプフラグが立ってたら
	if (m_isGround || m_isNextJump)
	{
		//ステート
		ChangeState(std::make_unique<PlayerJumpState>(*this,*m_animationController));
	}
}

void Player::JumpEnd()
{
	if (m_velocity.y > 0.0f)
	{
		m_velocity.y *= kJumpEndRate;
	}
}

void Player::Controll()
{
	m_isActive = true;
}

void Player::ExitControll()
{
	m_isActive = false;
}

void Player::UpdateModel()
{
	Matrix4x4 worldMat = m_transform.GetWorldMatrix();

	MV1SetMatrix(m_modelHandle, worldMat.ChangeDxMat());
}

void Player::ResetPlayerPos(const Vector3& pos)
{
	if (m_transform.GetPosition().y <= -10000.0f)
	{
		m_transform.SetPosition(pos);
	}
}

void Player::CaptureReleaseAction(const Vector3& pos)
{
	//ランダムに飛んでいく
	m_transform.SetPosition(pos);
	m_velocity.x = 0.0f;
	m_velocity.z = 0.0f;
	m_velocity.y = kJumpPower;
	m_isGround = false;
	m_animationController->Play(PlayerAnim::jump, false);
}
