#pragma once
#include "Engine/Core/Character.h"
#include "Engine/Collision/ICollider.h"
#include "Engine/Collision/Ray.h"
#include"Engine/AI/ITarget.h"
#include"Engine/Input/IControllable.h"
#include"Engine/Capture/ICaptureTarget.h"
namespace CrabEnemyAnim
{
	const std::wstring idle = L"MonsterArmature|Idle";
	const std::wstring walk = L"MonsterArmature|Walk";
	const std::wstring jump = L"MonsterArmature|Jump";
};

class CaptureManager;
class AnimationController;
class CrabEnemy : public Character, public ICollider,public ICaptureTarget
{
public:
	CrabEnemy(int enemyModel, int stageModel,CameraManager& camrea,CaptureManager& captureManager);
	~CrabEnemy();
	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	//落下時地上に戻る
	void ResetEnemyPos(const Vector3& pos);
	//------------------------------
	// IColliderの関数
	//------------------------------
	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other, CollisionResult& result) override;

	//------------------------------
	// IControllableの関数
	//------------------------------
	void Move(const Vector2& input)override;
	void Jump()override;

	//-----------------------------
	//ICameraTargetの関数
	//-----------------------------
	CameraAnchor GetCameraAnchor() const override;

	//------------------------------
	//ICaptureTargetの関数
	//------------------------------
	Matrix4x4 GetHatMatrix()const override;
private:

	int m_modelHandle;
	int m_stageModelHandle;

	int m_HeadFrameIndex;

	std::shared_ptr<AnimationController>m_animationController;

	CaptureManager& m_captureManager;
};

