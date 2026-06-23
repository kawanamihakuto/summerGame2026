#include "Player.h"

namespace
{
	const wchar_t* modelPath = L"data/model/Player.mv1";
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
	m_modelHandle = MV1LoadModel(modelPath);
}

void Player::End()
{
}

void Player::Update()
{
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
	DrawFormatString(16,32,0xffffff,L"pos : %f,%f,%f", m_transform.position.x, m_transform.position.y, m_transform.position.z);
	DrawFormatString(16,48,0xffffff,L"scale : %f,%f,%f", m_transform.scale.x, m_transform.scale.y, m_transform.scale.z);
}
