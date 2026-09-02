#include "SoundManager.h"
#include<DxLib.h>
#include<cassert>

std::unordered_map<std::string, int>SoundManager::m_bgmHandles;
std::unordered_map<std::string, int>SoundManager::m_seHandles;
int SoundManager::m_currentBGM = -1;
int SoundManager::m_fadeFrame = 0;
int SoundManager::m_fadeTimer = 0;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	for (auto se : m_seHandles)
	{
		DeleteSoundMem(se.second);
	}

	for (auto bgm : m_bgmHandles)
	{
		DeleteSoundMem(bgm.second);
	}
}

void SoundManager::Init()
{
	m_bgmHandles["title"] = LoadSoundMem(L"data/bgm/titleScene.mp3");
	assert(m_bgmHandles["title"] > -1);
	m_bgmHandles["game"] = LoadSoundMem(L"data/bgm/gameScene.mp3");
	assert(m_bgmHandles["game"] > -1);
	m_bgmHandles["clear"] = LoadSoundMem(L"data/bgm/clearScene.mp3");
	assert(m_bgmHandles["clear"] > -1);

	m_seHandles["pressAnyButton"] = LoadSoundMem(L"data/se/pressAnyButton.mp3");
	assert(m_seHandles["pressAnyButton"] > -1);
	m_seHandles["jump"] = LoadSoundMem(L"data/se/jump.mp3");
	assert(m_seHandles["jump"] > -1);
	m_seHandles["hatThrow"] = LoadSoundMem(L"data/se/hatThrow.mp3");
	assert(m_seHandles["hatThrow"] > -1);
	m_seHandles["capture"] = LoadSoundMem(L"data/se/capture.mp3");
	assert(m_seHandles["capture"] > -1);
	m_seHandles["captureEnd"] = LoadSoundMem(L"data/se/captureEnd.mp3");
	assert(m_seHandles["captureEnd"] > -1);
	m_seHandles["enemyDeath"] = LoadSoundMem(L"data/se/enemyDeath.mp3");
	assert(m_seHandles["enemyDeath"] > -1);
	m_seHandles["enemyTower"] = LoadSoundMem(L"data/se/enemyTower.mp3");
	assert(m_seHandles["enemyTower"] > -1);	
	m_seHandles["getStar"] = LoadSoundMem(L"data/se/getStar.mp3");
	assert(m_seHandles["getStar"] > -1);
	m_seHandles["playerHit"] = LoadSoundMem(L"data/se/playerHit.mp3");
	assert(m_seHandles["playerHit"] > -1);
}

void SoundManager::Update()
{
	if (m_fadeTimer > 0 && m_currentBGM != -1)
	{
		float rate = static_cast<float>(m_fadeTimer) / m_fadeFrame;
		int volume = static_cast<int>(160 * rate);

		ChangeVolumeSoundMem(volume, m_currentBGM);

		m_fadeTimer--;
		if (m_fadeTimer == 0)
		{
			StopSoundMem(m_currentBGM);
			m_currentBGM = -1;
		}
	}
}

void SoundManager::PlayBGM(const std::string& name, bool loop)
{
	auto it = m_bgmHandles.find(name);
	if (it == m_bgmHandles.end())
	{
		return;
	}

	m_fadeTimer = 0;
	m_fadeFrame = 0;

	if (m_currentBGM != -1)
	{
		StopSoundMem(m_currentBGM);
	}

	m_currentBGM = it->second;
	ChangeVolumeSoundMem(160, m_currentBGM);
	PlaySoundMem(m_currentBGM, loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
}

void SoundManager::StopBGM()
{
	if (m_currentBGM != -1)
	{
		StopSoundMem(m_currentBGM);
		m_currentBGM = -1;
	}
}

void SoundManager::FadeOutBGM(int frame)
{
	m_fadeFrame = frame;
	m_fadeTimer = frame;
	if (m_currentBGM != -1)
	{
		SetVolumeSoundMem(160, m_currentBGM);
	}
}

void SoundManager::PlaySE(const std::string& name, bool loop)
{
	auto it = m_seHandles.find(name);
	if (it == m_seHandles.end())
	{
		return;
	}
	if (loop)
	{
		PlaySoundMem(it->second, DX_PLAYTYPE_LOOP);
	}
	else
	{
		PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
	}
}

void SoundManager::StopSE(const std::string& name)
{
	auto it = m_seHandles.find(name);
	if (it == m_seHandles.end())
	{
		return;
	}
	StopSoundMem(it->second);
}