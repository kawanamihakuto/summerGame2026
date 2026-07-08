#pragma once
#include"Engine/Core/Character.h"
#include"Engine/Collision/ICollider.h"
#include"Engine/Collision/Ray.h"
#include"Engine/Camera/ICameraTarget.h"
#include"Engine/AI/ITarget.h"
#include"Engine/Input/IControllable.h"
namespace PlayerAnim
{
	const std::wstring idle = L"Armature|Idle";
	const std::wstring run = L"Armature|Run";
	const std::wstring jump = L"Armature|Jump";
};

class CameraManager;
class AnimationController;
class Player : public Character, public ICollider, public ICameraTarget , public ITarget ,public IControllable
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
	void OnCollision(ICollider& other) override;

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

	Transform* GetTransform();
	Vector3 GetGroundPlayerPos()const;
	
	void UpdateModel();

	//落下時地上に戻る
	void ResetPlayerPos(const Vector3& pos);

private:
	int m_modelHandle;

	int m_stageModelHandle;

	std::shared_ptr<AnimationController>m_animationController;

	Vector3 m_groundPlayerPos;
};

