#include "TItlePlayer.h"
#include"DxLib.h"
#include"Engine/Animation/AnimationController.h"
#include"Player.h"

namespace
{
	//帽子をかぶせたいフレーム名
	constexpr const wchar_t* kHeadFrameName = L"mixamorig:HeadTop_End";

	//帽子をかぶせたい位置のフレームからのオフセット
	constexpr Vector3 kHeadFrameOffsetMat = { 0.0f,-10.0f,-10.f };

	constexpr Vector3 kPosition = { 0.0f,-50.0f,0.0f };
}

TitlePlayer::TitlePlayer(int model)
{
	m_modelHandle = MV1DuplicateModel(model);

	m_transform.SetPosition(kPosition);

	MV1SetMatrix(m_modelHandle, m_transform.GetWorldMatrix().ChangeDxMat());

	m_animationController = std::make_shared<AnimationController>(m_modelHandle);

	//アニメーション追加
	m_animationController->AddAnimation(PlayerAnim::titleDance);	

	m_animationController->Play(PlayerAnim::titleDance);

	m_headFrameIndex = MV1SearchFrame(m_modelHandle, kHeadFrameName);
}

TitlePlayer::~TitlePlayer()
{
	MV1DeleteModel(m_modelHandle);
}

void TitlePlayer::Init()
{
	m_animationController->Play(PlayerAnim::titleDance);
}

void TitlePlayer::End()
{
}

void TitlePlayer::Update()
{
	Matrix4x4 worldMat = m_transform.GetWorldMatrix();

	MV1SetMatrix(m_modelHandle, worldMat.ChangeDxMat());
	m_animationController->Update();
}

void TitlePlayer::Draw()
{
	MV1DrawModel(m_modelHandle);
}

Matrix4x4 TitlePlayer::GetHatMatrix() const
{
	Matrix4x4 mat = Matrix4x4(MV1GetFrameLocalWorldMatrix(m_modelHandle, m_headFrameIndex));

	Matrix4x4 offsetMat = Matrix4x4::Translate(kHeadFrameOffsetMat);

	mat *= offsetMat;

	return mat;
}

ICaptureTarget* TitlePlayer::GetControllTarget()
{
	return this;
}

ICaptureTarget* TitlePlayer::GetHatAndCameraTarget()
{
	return this;
}

GameObject* TitlePlayer::GetGameObject()
{
	return this;
}

CameraAnchor TitlePlayer::GetCameraAnchor() const
{
	return CameraAnchor();
}

void TitlePlayer::Move()
{
}

void TitlePlayer::Jump()
{
}

void TitlePlayer::JumpEnd()
{
}

void TitlePlayer::Controll()
{
}

void TitlePlayer::ExitControll()
{
}
