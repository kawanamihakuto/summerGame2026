#pragma once
#include"Engine/Core/Character.h"
#include"Engine/Collision/ICollider.h"
#include"Engine/Collision/Ray.h"
#include"Engine/Camera/ICameraTarget.h"
#include"Engine/AI/ITarget.h"
#include"Engine/Input/IControllable.h"
#include"Engine/Capture/ICaptureTarget.h"

namespace PlayerAnim
{
	const std::wstring idle = L"Armature|Idle";
	const std::wstring run = L"Armature|Run";
	const std::wstring jump = L"Armature|Jump";
};

class CameraManager;
class AnimationController;
class Player : public Character, public ICollider, public ITarget ,public ICaptureTarget
{
public:
	Player(int playerModel, int stageModel,CameraManager& cameraManager);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	//-----------------------------
	// IColliderの関数
	//-----------------------------
	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;
	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;
	void OnCollision(ICollider& other,CollisionResult& result) override;

	//-----------------------------
	//ICameraTargetの関数
	//-----------------------------
	CameraAnchor GetCameraAnchor() const override;

	//-----------------------------
	//ITargetの関数
	//-----------------------------
	Vector3 GetPosition()const override;

	//----------------------------
	//IControllableの関数
	//----------------------------
	void Move(const Vector2& input)override;
	void Jump()override;
	void Controll()override;
	void ExitControll()override;

	//----------------------------
	//ICaptureTargetの関数
	//----------------------------
	Matrix4x4 GetHatMatrix()const override;
	ICaptureTarget* GetControllTarget()override;
	ICaptureTarget* GetHatAndCameraTarget()override;

	Transform* GetTransform();
	Vector3 GetGroundPlayerPos()const;
	
	void UpdateModel();

	//落下時地上に戻る
	void ResetPlayerPos(const Vector3& pos);

	Vector3 GetLastMoveDirection() { return { m_lastMoveInput.x,0.0f,m_lastMoveInput.z }; }
	//キャプチャー解除時のアクション
	void CaptureReleaseAction(const Vector3& pos);
private:
	int m_modelHandle;

	int m_stageModelHandle;

	int m_headFrameIndex;

	std::shared_ptr<AnimationController>m_animationController;

	Vector3 m_groundPlayerPos;

	bool m_isNextJump;
};

