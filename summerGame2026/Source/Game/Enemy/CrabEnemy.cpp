#include "CrabEnemy.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Animation/AnimationController.h"
#include"Engine/Math/Quaternion.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Camera/CameraManager.h"
#include"Engine/Capture/CaptureManager.h"
#include"Engine/Collision/CollisionManager.h"
#include"Engine/Collision/CapsuleCollider.h"
namespace
{
	constexpr float kCapsuleHeightOffset = 40.0f;
	constexpr float kCapsuleRadius = 40.0f;
	constexpr float kCapsuleHeight = 0.0f;

	constexpr Vector3 kSphereHeightOffset = {0.0f,40.0f ,0.0f};
	constexpr float kSphereRadius = 40.0f;

	//通常スピード
	constexpr float kSpeed = 2.0f;
	//キャプチャー時スピード
	constexpr float kCapturedSpeed = 4.0f;
	//マックススピード
	constexpr float kMaxSpeed = 3.0f;
	//ジャンプ力
	constexpr float kJumpPower = 16.0f;
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
	constexpr float kGroundRayHeightOffset = 85.0f;
	//レイの中心からの横オフセット
	constexpr float kGroundRayWidthOffset = 15.0f;
	//レイの長さ
	constexpr float kGroundRayLength = 90.0f;
	//レイの飛ばす方向
	Vector3 kGroundRayDirection = { 0.0f,-1.0f,0.0f };
	//オフセットの合成
	constexpr Vector3 kGroundRayOffsets[kGroundRayNum] =
	{
		{0.0f,kGroundRayHeightOffset,kGroundRayWidthOffset},
		{0.0f,kGroundRayHeightOffset,-kGroundRayWidthOffset},
		{kGroundRayWidthOffset,kGroundRayHeightOffset,0.0f},
		{-kGroundRayWidthOffset,kGroundRayHeightOffset,0.0f},
	};

	//帽子をかぶせたいフレーム
	constexpr const wchar_t* kHeadFrameName = L"Head3";
}

CrabEnemy::CrabEnemy(int enemyModel, int stageModel, CameraManager& camera, CaptureManager& captureManager, const Vector3& pos) :
	Character(camera),
	m_captureManager(captureManager),
	m_capsuleColliderHeight(kCapsuleHeight),
	m_towerNum(1)
{
	//自分のモデル複製
	m_modelHandle = MV1DuplicateModel(enemyModel);
	//ステージモデル
	m_stageModelHandle = stageModel;
	//ステート
	m_state = State::ai;
	//ステージ用コライダー生成
//	m_stageCollider = std::make_unique<SphereCollider>(m_transform.position, kSphereRadius);
	m_stageCollider = std::make_unique<CapsuleCollider>(m_transform.position + Vector3{ 0.0f, kCapsuleHeightOffset, 0.0f }, kCapsuleRadius, kCapsuleHeight);
	//キャラクター用コライダー生成
	m_bodyCollider = std::make_unique<SphereCollider>(m_transform.position, kSphereRadius);
	//レイ生成
	m_ray.resize(kGroundRayNum);
	for (int i = 0; i < kGroundRayNum; i++)
	{
		m_ray[i] = std::make_unique<Ray>(m_transform.position, kGroundRayDirection, kGroundRayLength, kGroundRayOffsets[i]);
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
	Vector3 vec;
	Vector3 dir;
	switch (m_state)
	{
	case State::ai:
		//ターゲットにへのベクトル
		vec = m_captureManager.GetTarget()->GetHatMatrix().GetTranslation() - m_transform.position;
		//Y成分なくすよ
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

	case State::controll:
		//入力取得
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
		//速度決める
		UpdateMove();
		//回転
		UpdateRotate(kModelRotationOffset);
		//重力
		Gravity();
		//コライダー系の更新
		ColliderUpdate();
		break;

	case State::tower:
		//コライダーを動かす
		m_stageCollider->Update(m_transform.position + Vector3{0.0f, kCapsuleHeightOffset, 0.0f} + m_velocity);
		m_bodyCollider->Update(m_transform.position + kSphereHeightOffset+ m_velocity);
		//レイを動かす
		for (auto& ray : m_ray)
		{
			ray->Update(m_transform.GetPosition() + m_velocity);
		}
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

	m_towerNum = 1;
	CheckTowerNum();

	//奈落に落ちたとき用
	ResetEnemyPos({ 0.0f,0.0f,0.0f });

	//モデルに適用させるよ
	MV1SetPosition(m_modelHandle, m_transform.GetPosition());
	MV1SetRotationMatrix(m_modelHandle, m_transform.GetRotationMatrix().ChangeDxMat());
	m_animationController->Update();
}

void CrabEnemy::Draw()
{
	//モデル描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	//コライダーとか描画
	m_stageCollider->Draw();
	m_bodyCollider->Draw();
	for (auto& ray : m_ray)
	{
		ray->Draw();
	}
#endif // _DEBUG
}

void CrabEnemy::ChangeState(State nextState)
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
	else
	{
		if (m_upper)
		{
			m_towerNum++;
			m_upper->CheckTowerNum();
		}
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
	m_stageCollider->Update(m_transform.position + Vector3{ 0.0f,kCapsuleHeightOffset,0.0f } + m_velocity);
	m_bodyCollider->Update(m_transform.position + kSphereHeightOffset +m_velocity);

	ResolveWallVelocity(m_stageModelHandle);

	for (auto& ray : m_ray)
	{
		ray->Update(m_transform.GetPosition() + m_velocity);
	}

	GroundCollision(m_stageModelHandle);
}

const Collider& CrabEnemy::GetCollider() const
{
	return *m_bodyCollider;
}

const Ray& CrabEnemy::GetRay() const
{
	return *m_ray[0];
}

CollisionLayer CrabEnemy::GetCollisionLayer() const
{
	if (m_state == State::controll)
	{
		return CollisionLayers::kControllEnemy;
	}
	else if (m_state == State::ai)
	{
		return CollisionLayers::kEnemy;
	}
	else if (m_state == State::tower)
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
		CollisionLayers::kControllEnemy;
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
				Destroy();
			}
		}
	}

	//敵同士の押し戻し
	if (m_state == State::ai)
	{
		if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
		{
			if (auto crab = dynamic_cast<CrabEnemy*>(&other))
			{
				if (crab->m_state != State::tower)
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
					SetUpper(crab);
					crab->SetLower(this);

					m_state = State::controll;
					crab->ChangeState(State::tower);
				}
			}
		}
	}
}

void CrabEnemy::Move(const Vector2& input)
{
	auto& camera = m_cameraManager.GetTransfrom();
	//移動ベクトル生成
	m_moveInput = Vector3::Zero();
	m_moveInput += Vector3{ camera.Right().x,0.0f,camera.Right().z } * input.x;
	m_moveInput += Vector3{ camera.Forward().x,0.0f,camera.Forward().z } * input.y;
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

void CrabEnemy::Controll()
{
	m_state = State::controll;
}

void CrabEnemy::ExitControll()
{
	m_state = State::ai;
}

CameraAnchor CrabEnemy::GetCameraAnchor() const
{
	return { m_transform };
}

Matrix4x4 CrabEnemy::GetHatMatrix() const
{
	Matrix4x4 mat = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_HeadFrameIndex);
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
