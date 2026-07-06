#pragma once
#include"Engine/Core/Character.h"
#include"Engine/Core/ICollider.h"
#include"Engine/Collision/CapsuleCollider.h"
#include"Engine/Collision/Ray.h"
#include"Engine/Core/ICameraTarget.h"
namespace PlayerAnim
{
	const std::wstring idle = L"Armature|Idle";
	const std::wstring run = L"Armature|Run";
	const std::wstring jump = L"Armature|Jump";
};

class CameraManager;
class AnimationController;
class Player : public Character, public ICollider, public ICameraTarget
{
public:
	Player(int playerModel, int stageModel,CameraManager& cameraManager);
	~Player();

	void Init()override;
	void End()override;
	void Update()override;
	void Draw()override;

	const Collider& GetCollider()const override;
	const Ray& GetRay()const override;

	CameraAnchor GetCameraAnchor() const override;

	CollisionLayer GetCollisionLayer()const override;
	CollisionLayer GetCollisionMask()const override;

	void OnCollision(ICollider& other) override;

	Transform* GetTransform();

	Vector3 GetGroundPlayerPos()const;
	
	void Gravity();
	void WallCollision();
	void GroundCollision();
	void UpdateModel();

	void ResetPlayerPos(const Vector3& pos);

private:
	int m_modelHandle;

	int m_stageModelHandle;

	std::shared_ptr<AnimationController>m_animationController;

	CapsuleCollider m_capsuleCol;
	std::vector<Ray> m_ray;

	bool m_isGround;

	Vector3 m_groundPlayerPos;
};

