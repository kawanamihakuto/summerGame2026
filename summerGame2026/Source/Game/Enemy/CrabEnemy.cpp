#include "CrabEnemy.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Quaternion.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Capture/CaptureManager.h"
#include"Engine/Collision/CollisionManager.h"
#include"Engine/Collision/CapsuleCollider.h"
#include"Engine/Collision/Ray.h"
namespace
{
	//高さ
	constexpr float kHeight = 70.0f;

	//カプセルの高さオフセット
	constexpr float kCapsuleHeightOffset = 15.0f;
	//カプセル半径
	constexpr float kCapsuleRadius = 40.0f;

	//球高さオフセット
	constexpr Vector3 kSphereHeightOffset = { 0.0f,40.0f ,0.0f };
	//球半径
	constexpr float kSphereRadius = 40.0f;

	//通常スピード
	constexpr float kSpeed = 2.0f;
	//キャプチャー時スピード
	constexpr float kCapturedSpeed = 4.0f;
	//マックススピード
	constexpr float kMaxSpeed = 3.0f;
	//ジャンプ力
	constexpr float kJumpPower = 16.0f;
	//ジャンプ終了時のvelocityを減らす割合
	constexpr float kJumpEndRate = 0.5f;
	//追いかける距離
	constexpr float kChaseLength = 500.0f;

	//モデル回転のオフセット
	const Quaternion kModelRotationOffset = Quaternion::AngleAxis(DX_PI_F, Vector3::Up());
	//拡大率
	constexpr Vector3 kScale = { 0.5f,0.5f,0.5f };
	//重なるときの頭の位置
	constexpr Vector3 kTowerHeight = { 0.0f,70.0f,0.0f };
	//地面判定用のレイの数
	constexpr int kGroundRayNum = 4;
	//レイの高さオフセット
	constexpr float kGroundRayHeightOffset = 5.0f;
	//レイの中心からの横オフセット幅
	constexpr float kGroundRayWidthOffset = 15.0f;
	//レイの長さ
	constexpr float kGroundRayLength = 90.0f;
	//レイの飛ばす方向
	Vector3 kGroundRayDirection = { 0.0f,-1.0f,0.0f };
	//横オフセットたち
	constexpr Vector3 kGroundRayWidthOffsets[kGroundRayNum] =
	{
		{0.0f                  , 0.0f , kGroundRayWidthOffset  },
		{0.0f                  , 0.0f , -kGroundRayWidthOffset },
		{kGroundRayWidthOffset , 0.0f , 0.0f                   },
		{-kGroundRayWidthOffset, 0.0f , 0.0f                   },
	};

	//帽子をかぶせたいフレーム名
	constexpr const wchar_t* kHeadFrameName = L"Head3";

	//無敵時間(フレーム)
	constexpr float kInvincibleFrame = 60.0f * 3.0f;
}

CrabEnemy::CrabEnemy(int enemyModel, int stageModel, CameraManager& camera, CaptureManager& captureManager, const Vector3& pos) :
	Character(camera),
	m_captureManager(captureManager),
	m_towerNum(1)
{
	//自分のモデル複製
	m_modelHandle = MV1DuplicateModel(enemyModel);
	//ステージモデル
	m_stageModelHandle = stageModel;
	//ステート
	m_state = ControllState::ai;
	//ステージ用コライダー生成
	m_stageCollider = std::make_unique<CapsuleCollider>(m_transform.position + Vector3{ 0.0f,kHeight / 2.0f + kCapsuleHeightOffset,0.0f }, kCapsuleRadius, 0.0f);
	//キャラクター用コライダー生成
	m_bodyCollider = std::make_unique<SphereCollider>(m_transform.position, kSphereRadius);
	//レイ生成
	m_ray.resize(kGroundRayNum);
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i] = std::make_unique<Ray>(m_transform.position, kGroundRayDirection, kGroundRayLength, kGroundRayWidthOffsets[i], kHeight + kGroundRayHeightOffset);
	}
	//頭のフレームのインデックス
	m_HeadFrameIndex = MV1SearchFrame(m_modelHandle, kHeadFrameName);
	//ポジション初期化
	m_transform.SetPosition(pos);

	//アニメーションコントローラー生成
	m_animationController = std::make_shared<AnimationController>(m_modelHandle);
	//アニメーション追加
	m_animationController->AddAnimation(CrabEnemyAnim::idle);
	m_animationController->AddAnimation(CrabEnemyAnim::walk);
	m_animationController->AddAnimation(CrabEnemyAnim::jump);
	//アニメーション再生
	m_animationController->Play(CrabEnemyAnim::idle);
	//拡大縮小
	MV1SetScale(m_modelHandle, kScale);
	//モデル更新
	ModelUpdate();
	//当たり判定更新
	ColliderUpdate();
}

CrabEnemy::~CrabEnemy()
{
	//モデル解放
	MV1DeleteModel(m_modelHandle);
}

void CrabEnemy::Init()
{

}

void CrabEnemy::End()
{
}

void CrabEnemy::Update()
{
	m_towerNum = 1;
	//重なってる数を数える
	CheckTowerNum();
	if (auto capsule = dynamic_cast<CapsuleCollider*>(m_stageCollider.get()))
	{
		//カプセルの高さを調整
		capsule->SetHeight(kHeight * (m_towerNum - 1));
	}

	for (int i = 0; i < kGroundRayNum; i++)
	{
		//レイの高さを調整
		m_ray[i]->SetOffSetAndLength(kGroundRayWidthOffsets[i], (kHeight * m_towerNum) + kGroundRayHeightOffset, (kHeight * m_towerNum) + kGroundRayHeightOffset);
	}

	Vector3 vec;
	Vector3 dir;
	switch (m_state)
	{
	case ControllState::ai:
		//ターゲットにへのベクトル
		vec = m_captureManager.GetTarget()->GetGameObject()->GetTransform().GetPosition() - m_transform.position;

		//Y成分消しとく
		vec.y = 0.0f;
		//近すぎない、もしくは索敵範囲内だったら
		if (vec.Length() > 1.0f && vec.Length() < kChaseLength)
		{
			//正規化
			vec.Normalize();
			//歩きアニメーション
			m_animationController->Play(CrabEnemyAnim::walk);
		}
		else
		{
			//ベクトルを0にする
			vec = Vector3::Zero();
			//待機アニメーション
			m_animationController->Play(CrabEnemyAnim::idle);
		}

		//速度設定
		m_velocity.x = vec.x * kSpeed;
		m_velocity.z = vec.z * kSpeed;
		//回転
		UpdateRotate(kModelRotationOffset);
		//重力
		Gravity();
		//コライダー系の更新
		ColliderUpdate();
		break;

	case ControllState::controll:
		//入力取得
		Move();
		dir = m_moveInput;
		//入力があったら
		if (dir.Length() > 0.0f)
		{
			//地面にいたら
			if (m_isGround)
			{
				//歩きアニメーション
				m_animationController->Play(CrabEnemyAnim::walk);
			}
		}
		//入力が無かったら
		if (dir.Length() == 0)
		{
			//地面にいたら
			if (m_isGround)
			{
				//待機アニメーション
				m_animationController->Play(CrabEnemyAnim::idle);
			}
		}

		if (m_input.isJumpDown)
		{
			Jump();
		}

		if (m_input.isJumpUp)
		{
			JumpEnd();
		}

		//速度決める
		UpdateMove();
		//回転
		UpdateRotate(kModelRotationOffset);
		//重力
		Gravity();
		//コライダー系の更新
		ColliderUpdate();
		break;

	case ControllState::tower:
		//コライダーを動かす
		m_stageCollider->Update(m_transform.position + Vector3{ 0.0f,kHeight / 2.0f + kCapsuleHeightOffset,0.0f } + m_velocity);
		m_bodyCollider->Update(m_transform.position + kSphereHeightOffset + m_velocity);
		//レイを動かす
		for (auto& ray : m_ray)
		{
			ray->Update(m_transform.GetPosition() + m_velocity);
		}

		m_animationController->Play(CrabEnemyAnim::idle);
		break;
	}
	//空中にいるときに動く
	if (!m_isGround)
	{
		m_transform.Translate(m_velocity);
	}
	//一番下なら
	if (m_lower == nullptr)
	{
		//上のやつがいたら
		if (m_upper)
		{
			//上のやつらをついてこさせる
			m_upper->FollowTower(this);
		}
	}

	//奈落に落ちたとき用
	ResetEnemyPos({ 0.0f,0.0f,0.0f });

	ModelUpdate();
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

void CrabEnemy::Draw()
{
	if (m_isInvincible && m_InvincibleFrameCount % 10 < 5)
	{
		MV1SetOpacityRate(m_modelHandle, 0.2f);
	}
	//モデル描画
	MV1DrawModel(m_modelHandle);

	MV1SetOpacityRate(m_modelHandle, 1.0f);

#ifdef _DEBUG
	//コライダーとか描画
	if (m_state != ControllState::tower)
	{
		m_stageCollider->Draw();

		for (auto& ray : m_ray)
		{
			ray->Draw();
		}
	}
	//	m_bodyCollider->Draw();

#endif // _DEBUG
}

void CrabEnemy::ChangeState(ControllState nextState)
{
	m_state = nextState;
}

void CrabEnemy::SetUpper(CrabEnemy* upper)
{
	m_upper = upper;
}

void CrabEnemy::SetLower(CrabEnemy* lower)
{
	m_lower = lower;
}

CrabEnemy* CrabEnemy::GetTop()
{
	CrabEnemy* current = this;
	//一番上のやつまで回す
	while (current->m_upper)
	{
		current = current->m_upper;
	}

	return current;
}

CrabEnemy* CrabEnemy::GetBottom()
{
	CrabEnemy* current = this;
	//一番下のやつまで回す
	while (current->m_lower)
	{
		current = current->m_lower;
	}

	return current;
}

void CrabEnemy::FollowTower(CrabEnemy* lower)
{
	m_transform.SetPosition(lower->GetTransform().GetPosition() + kTowerHeight);
	m_transform.SetRotate(lower->GetTransform().GetRotation());
	//一番上のやつまで回す
	if (m_upper)
	{
		m_upper->FollowTower(this);
	}
}

void CrabEnemy::CheckTowerNum()
{
	if (m_lower == nullptr)
	{
		if (m_upper)
		{
			m_towerNum++;
			m_upper->CheckTowerNum();
		}
	}

	if (m_lower)
	{
		if (m_upper)
		{
			GetBottom()->m_towerNum++;
			m_upper->CheckTowerNum();
		}
	}
}

void CrabEnemy::InvincibleTower()
{
	CrabEnemy* current = this;
	//一番上のやつまで回す
	while (current->m_upper)
	{
		current = current->m_upper;
		current->m_isInvincible = true;
	}

	current = this;
	//一番下のやつまで回す
	while (current->m_lower)
	{
		current = current->m_lower;
		current->m_isInvincible = true;
	}
}

void CrabEnemy::ResetEnemyPos(const Vector3& pos)
{
	if (m_transform.GetPosition().y <= -10000.0f)
	{
		m_transform.SetPosition(pos);
	}
}

void CrabEnemy::ColliderUpdate()
{
	m_stageCollider->Update(m_transform.position + (Vector3{ 0.0f,kHeight / 2.0f,0.0f } * static_cast<float>(m_towerNum)) + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f } + m_velocity);
	m_bodyCollider->Update(m_transform.position + kSphereHeightOffset + m_velocity);

	ResolveWallVelocity(m_stageModelHandle);

	for (auto& ray : m_ray)
	{
		ray->Update(m_transform.GetPosition() + m_velocity);
	}

	GroundCollision(m_stageModelHandle);
}

void CrabEnemy::ModelUpdate()
{
	//モデルに適用させるよ
	MV1SetPosition(m_modelHandle, m_transform.GetPosition());
	MV1SetRotationMatrix(m_modelHandle, m_transform.GetRotationMatrix().ChangeDxMat());
}

const Collider& CrabEnemy::GetCollider() const
{
	return *m_bodyCollider;
}

//const Ray& CrabEnemy::GetRay() const
//{
//	return *m_ray[0];
//}

CollisionLayer CrabEnemy::GetCollisionLayer() const
{
	if (m_state == ControllState::controll)
	{
		return CollisionLayers::kControllEnemy;
	}
	else if (m_state == ControllState::ai)
	{
		return CollisionLayers::kEnemy;
	}
	else if (m_state == ControllState::tower)
	{
		return CollisionLayers::kEnemy;
	}
	else
	{
		return CollisionLayers::kEnemy;
	}
}

CollisionLayer CrabEnemy::GetCollisionMask() const
{
	return CollisionLayers::kPlayer |
		CollisionLayers::kHat |
		CollisionLayers::kEnemy |
		CollisionLayers::kControllEnemy |
		CollisionLayers::kStar;
}

void CrabEnemy::OnCollision(ICollider& other, CollisionResult& result)
{
	//プレイヤーに踏まれるよ
	if (other.GetCollisionLayer() == CollisionLayers::kPlayer)
	{
		if (auto obj = dynamic_cast<PhysicsObject*>(&other))
		{
			if (result.normal.y > 0.5f && obj->GetVelocity().y < 0.0f)
			{
				//上のやつとか下のやつをつなげるよ
				if (m_lower)
				{
					if (m_upper)
					{
						m_lower->m_upper = m_upper;
						m_upper->m_lower = m_lower;
					}
					else
					{
						m_lower->m_upper = nullptr;
					}
				}
				else
				{
					if (m_upper)
					{
						m_upper->m_lower = nullptr;
						m_upper->ChangeState(ControllState::ai);
					}
				}
				Destroy();
			}
		}
	}

	//敵同士の押し戻し
	if (m_state == ControllState::ai)
	{
		if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
		{
			if (auto crab = dynamic_cast<CrabEnemy*>(&other))
			{
				if (crab->m_state != ControllState::tower)
				{
					Vector3 push = result.normal * result.penetration;

					auto hits = m_stageCollider->CheckWallCollision(m_stageModelHandle);

					for (const auto& hit : hits)
					{
						if (hit.normal.y > 0.7f)
							continue;

						float dot = push.Dot(hit.normal);

						if (dot < 0.0f)
						{
							push -= hit.normal * dot;
						}
					}

					m_transform.Translate(push);
				}
			}
		}
	}

	//重なる処理
	if (other.GetCollisionLayer() == CollisionLayers::kControllEnemy)
	{
		if (auto obj = dynamic_cast<PhysicsObject*>(&other))
		{
			if (result.normal.y < -0.7f && obj->GetVelocity().y < 0.0f)
			{
				if (auto crab = dynamic_cast<CrabEnemy*>(obj))
				{
					if (m_upper == nullptr)
					{
						if (crab->m_lower == nullptr)
						{
							SetUpper(crab);
							crab->SetLower(this);

							ChangeState(ControllState::tower);
							GetBottom()->ChangeState(ControllState::controll);
							crab->ChangeState(ControllState::tower);
						}
					}
				}
			}
		}
	}

	if (m_state == ControllState::controll)
	{
		if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
		{
			if (!m_isInvincible)
			{
				if (result.normal.y > -0.7f && m_velocity.y >= 0.0f)
				{
					SetCaptureRelease(true);
				}
			}
		}
	}
}

void CrabEnemy::Move()
{
	auto& camera = m_cameraManager.GetTransfrom();
	//移動ベクトル生成
	m_moveInput = Vector3::Zero();
	m_moveInput += Vector3{ camera.Right().x,0.0f,camera.Right().z } * m_input.move.x;
	m_moveInput += Vector3{ camera.Forward().x,0.0f,camera.Forward().z } * m_input.move.y;
	m_moveInput.Normalize();
	m_moveInput.x *= kCapturedSpeed;
	m_moveInput.z *= kCapturedSpeed;
	m_moveInput.y = 0.0f;
}

void CrabEnemy::Jump()
{
	//地面にいたらジャンプするよ
	if (m_isGround)
	{
		m_velocity.y = kJumpPower;
		m_isGround = false;
		m_animationController->Play(CrabEnemyAnim::jump, false);
	}
}

void CrabEnemy::JumpEnd()
{
	if (m_velocity.y > 0.0f)
	{
		m_velocity *= kJumpEndRate;
	}
}

void CrabEnemy::Controll()
{
	m_state = ControllState::controll;
	
	m_isInvincible = true;
	InvincibleTower();
}

void CrabEnemy::ExitControll()
{
	m_state = ControllState::ai;
	m_isInvincible = false;
	m_InvincibleFrameCount = 0;
}

CameraAnchor CrabEnemy::GetCameraAnchor() const
{
	return { m_transform };
}

Matrix4x4 CrabEnemy::GetHatMatrix() const
{
	Matrix4x4 mat = Matrix4x4(MV1GetFrameLocalWorldMatrix(m_modelHandle, m_HeadFrameIndex));
	return mat;
}

ICaptureTarget* CrabEnemy::GetControllTarget()
{
	return GetBottom();
}

ICaptureTarget* CrabEnemy::GetHatAndCameraTarget()
{
	return GetTop();
}

GameObject* CrabEnemy::GetGameObject()
{
	return GetTop();
}
