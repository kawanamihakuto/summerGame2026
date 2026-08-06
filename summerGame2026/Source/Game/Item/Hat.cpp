#include "Hat.h"
#include"Engine/Core/Precompiled.h"
#include"Engine/Collision/SphereCollider.h"
#include"Engine/Capture/ICaptureTarget.h"
#include"Engine/Math/Transform.h"
#include"Engine/Math/Quaternion.h"
#include"Engine/Collision/CollisionManager.h"
namespace
{
	//飛んでいくスピード
	constexpr float kGoSpeed = 18.0f;
	//戻ってくるスピード
	constexpr float kBackSpeed = 30.0f;
	//飛んでいくフレーム数
	constexpr int kGoFrame = 30;
	//待機状態フレーム数
	constexpr int kWaitFrame = 70;

	//当たり判定の半径
	constexpr float kSphereRadius = 60.0f;
	//モデルスケール
	constexpr Vector3 kModelScale = { 60.0f,60.0f,60.0f };
}

Hat::Hat(int modelHandle, int stageModelHandle, ICaptureTarget* target) :
	m_targetPos({}),
	m_target(target),
	m_endPos({}),
	m_count(0),
	m_direction({}),
	m_CaptureFlag(false)
{
	//モデル複製
	m_modelHandle = MV1DuplicateModel(modelHandle);
	//ステージとの当たり判定用モデル
	m_stageModelHandle = stageModelHandle;
	//ステージとの当たり判定用コライダー生成
	m_stageCollider = std::make_unique<SphereCollider>(m_transform.position, kSphereRadius);
	//最初は帽子かぶってる状態
	m_state = HatState::have;
}

Hat::~Hat()
{
	MV1DeleteModel(m_modelHandle);
}

void Hat::Init()
{
	//ポジション設定
	m_transform = Transform::FromMatrix(m_target->GetHatMatrix());
}

void Hat::End()
{
}

void Hat::Update()
{
	switch (m_state)
	{
		//持ってるとき
	case HatState::have:
		m_transform = Transform::FromMatrix(m_target->GetHatMatrix());
		m_stageCollider->SetIsActive(false);
		break;
		//飛んで行ってるとき
	case HatState::go:
		//方向正規化
		m_direction.Normalize();
		//速度計算
		m_velocity = m_direction * kGoSpeed;
		//コライダー更新
		m_stageCollider->Update(m_transform.position + m_velocity);
		//壁との押し戻し
		ResolveWallVelocity(m_stageModelHandle);
		//位置計算
		m_transform.Translate(m_velocity);
		//飛んでいくフレームカウント
		if (m_count++ >= kGoFrame)
		{
			m_state = HatState::wait;
			m_count = 0;
		}
		break;
		//待機状態
	case HatState::wait:
		//待機状態フレームカウント
		if (m_count++ >= kWaitFrame)
		{
			m_state = HatState::back;
			m_count = 0;
		}
		break;
		//戻ってくる状態
	case HatState::back:
		//ターゲットの位置に向かう
		m_direction = m_target->GetHatMatrix().GetTranslation() - m_transform.position;
		//近づいたらかぶる状態にする
		if (m_direction.Length() <kBackSpeed)
		{
			m_state = HatState::have;
			m_count = 0;
			break;
		}
		//方向正規化
		m_direction.Normalize();
		//速度計算
		m_velocity = m_direction * kBackSpeed;
		//コライダー更新
		m_stageCollider->Update(m_transform.position + m_velocity);
		//位置計算
		m_transform.Translate(m_velocity);
		break;
	}
	
	//モデル制御
	MV1SetPosition(m_modelHandle,m_transform.position);
	MV1SetRotationMatrix(m_modelHandle, m_transform.rotation.ToMatrix().ChangeDxMat());
	MV1SetScale(m_modelHandle, kModelScale);
}

void Hat::Draw()
{
	//モデル描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG
	m_stageCollider->Draw();
#endif // _DEBUG

}

void Hat::Throw(const Vector3& dir)
{
	//帽子を持っていたら投げれる
	if (m_state == HatState::have)
	{
		m_direction = dir.Normalized();
		m_stageCollider->SetIsActive(true);
		m_state = HatState::go;
		m_count = 0;
	}
}

const Collider& Hat::GetCollider() const
{
	return *m_stageCollider;
}

//const Ray& Hat::GetRay() const
//{
//	return *m_ray[0];
//}

CollisionLayer Hat::GetCollisionLayer() const
{
	return CollisionLayers::kHat;
}

CollisionLayer Hat::GetCollisionMask() const
{
	return CollisionLayers::kEnemy;
}

void Hat::OnCollision(ICollider& other,CollisionResult& result)
{
	if (m_state == HatState::have)
	{
		return;
	}

	//敵と当たったら
	if (other.GetCollisionLayer() == CollisionLayers::kEnemy)
	{
		if (auto cast = dynamic_cast<ICaptureTarget*>(&other))
		{
			//キャプチャーするよ
			SetTarget(cast);
			m_state = HatState::have;
			m_CaptureFlag = true;
		}
	}
}

void Hat::SetTarget(ICaptureTarget* target)
{
	m_target = target->GetHatAndCameraTarget();
	m_state = HatState::have;
	m_transform = Transform::FromMatrix(m_target->GetHatMatrix());
	m_stageCollider->Update(m_transform.position);
	m_stageCollider->SetIsActive(false);
}

ICaptureTarget* Hat::GetCaptureTarget()
{
	return m_target;
}
