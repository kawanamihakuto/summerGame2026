#pragma once
#include"Engine/Core/Character.h"
#include"Engine/Collision/ICollider.h"
#include"Engine/Collision/Ray.h"
#include"Engine/Camera/ICameraTarget.h"
#include"Engine/Input/IControllable.h"
#include"Engine/Capture/ICaptureTarget.h"

namespace PlayerAnim
{
	const std::wstring idle = L"Armature|Idle";
	const std::wstring walk = L"Armature|Walk";
	const std::wstring run = L"Armature|Run";
	const std::wstring jump = L"Armature|Jump";
	const std::wstring stunned = L"Armature|Stunned";
};

class PlayerStateBase;
class CameraManager;
class AnimationController;
class Player : public Character, public ICollider,public ICaptureTarget
{
public:
	Player(int playerModel, int stageModel,CameraManager& cameraManager);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	//ステートの遷移
	void ChangeState(std::unique_ptr<PlayerStateBase> newState);
	//ジャンプ時のまとめ
	void StartJamp();
	//走っているかどうか
	bool IsRun() { return m_isRun; }
	//x,z成分の動きを止める
	void StopMove();

	//-----------------------------
	// IColliderの関数
	//-----------------------------
	const Collider& GetCollider()const override;
//	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other,CollisionResult& result) override;

	//-----------------------------
	//ICameraTargetの関数
	//-----------------------------
	CameraAnchor GetCameraAnchor() const override;

	//----------------------------
	//IControllableの関数
	//----------------------------
	void Move()override;
	void Jump()override;
	void JumpEnd()override;
	void Controll()override;
	void ExitControll()override;

	//----------------------------
	//ICaptureTargetの関数
	//----------------------------
	Matrix4x4 GetHatMatrix()const override;
	ICaptureTarget* GetControllTarget()override;
	ICaptureTarget* GetHatAndCameraTarget()override;
	GameObject* GetGameObject()override;
	
	//モデル更新まとめ
	void UpdateModel();
	//落下時地上に戻る
	void ResetPlayerPos(const Vector3& pos);
	//最後の入力を取る
	Vector3 GetLastMoveDirection() { return { m_lastMoveInput.x,0.0f,m_lastMoveInput.z }; }
	//キャプチャー解除時のアクション
	void CaptureReleaseAction(const Vector3& pos);
private:
	std::unique_ptr<PlayerStateBase>m_state;

	int m_modelHandle;

	int m_stageModelHandle;

	int m_headFrameIndex;

	std::shared_ptr<AnimationController>m_animationController;

	bool m_isNextJump;

	bool m_isRun;
};

