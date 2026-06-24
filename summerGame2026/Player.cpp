#include "Player.h"
#include"Engine/Core/PreCompiled.h"
#include"Engine/Core/InputManager.h"
#include"CameraManager.h"
namespace
{
	const wchar_t* kModelPath = L"data/model/Player.mv1";
	constexpr float kSpeed = 5.0f;
}

Player::Player() :
	m_modelHandle(-1)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_modelHandle = MV1LoadModel(kModelPath);
}

void Player::End()
{
}

void Player::Update()
{
	auto& input = InputManager::GetInstance();
	auto& cameraTransform = CameraManager::GetInstance().GetTransfrom();

	Vector2 leftStick = input.GetLeftStick();
	Vector3 move = { 0.0f,0.0f,0.0f };
	move += cameraTransform.Right() * leftStick.x * kSpeed;
	move += cameraTransform.Forward() * leftStick.y * kSpeed;
	move.y = 0.0f;

	m_transform.Translate(move);

	Matrix4x4 worldMat = m_transform.GetWorldMatrix();

	MV1SetMatrix(m_modelHandle, worldMat.ChangeDxMat());
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
	DrawFormatString(16,32,0xffffff,L"pos : %f,%f,%f", m_transform.position.x, m_transform.position.y, m_transform.position.z);
	DrawFormatString(16,48,0xffffff,L"scale : %f,%f,%f", m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);
}

Transform* Player::GetTransform()
{
	return &m_transform;
}
