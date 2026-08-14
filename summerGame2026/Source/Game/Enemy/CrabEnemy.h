#pragma once
#include "Engine/Core/Character.h"
#include "Engine/Collision/ICollider.h"
#include"Engine/Capture/ICaptureTarget.h"

//アニメーション名たち
namespace CrabEnemyAnim
{
	const std::wstring idle = L"MonsterArmature|Idle";
	const std::wstring walk = L"MonsterArmature|Walk";
	const std::wstring jump = L"MonsterArmature|Jump";
};

//制御ステート
enum class ControllState
{
	ai,
	controll,
	tower
};

class CaptureManager;
class AnimationController;
class CrabEnemy : public Character, public ICollider,public ICaptureTarget
{
public:
	CrabEnemy(int enemyModel, int stageModel,CameraManager& camrea,CaptureManager& captureManager,const Vector3& pos);
	~CrabEnemy();
	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	void ChangeState(ControllState nextState);

	//上に乗っている奴を保存
	void SetUpper(CrabEnemy* upper);
	//下にいる奴を保存
	void SetLower(CrabEnemy* lower);
	//一番上のやつ
	CrabEnemy* GetTop();
	//一番下のやつ
	CrabEnemy* GetBottom();
	//上のやつをついていかせる
	void FollowTower(CrabEnemy* lower);
	//タワーの敵の数
	void CheckTowerNum();
	//タワー全体を無敵化する
	void InvincibleTower();

	//落下時地上に戻る
	void ResetEnemyPos(const Vector3& pos);

	//コライダー系のまとめ
	void ColliderUpdate();

	//モデル更新
	void ModelUpdate();
	//------------------------------
	// IColliderの関数
	//------------------------------
	const Collider& GetCollider()const override;
//	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other, CollisionResult& result) override;

	//------------------------------
	// IControllableの関数
	//------------------------------
	void Move()override;
	void Jump()override;
	void JumpEnd()override;
	void Controll()override;
	void ExitControll()override;

	//-----------------------------
	//ICameraTargetの関数
	//-----------------------------
	CameraAnchor GetCameraAnchor() const override;

	//------------------------------
	//ICaptureTargetの関数
	//------------------------------
	Matrix4x4 GetHatMatrix()const override;
	ICaptureTarget* GetControllTarget()override;
	ICaptureTarget* GetHatAndCameraTarget()override;
	GameObject* GetGameObject()override;

private:
	int m_modelHandle;
	int m_stageModelHandle;

	int m_towerNum;

	int m_HeadFrameIndex;

	std::shared_ptr<AnimationController>m_animationController;

	CaptureManager& m_captureManager;

	ControllState m_state;

	CrabEnemy* m_upper = nullptr;
	CrabEnemy* m_lower = nullptr;
};

