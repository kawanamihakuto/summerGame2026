#pragma once
#include"Engine/Core/Precompiled.h"
#include<unordered_map>

class SoundManager
{
public:
	SoundManager();
	virtual ~SoundManager();

	static void Init();
	static void Update();

	//BGM
	static void PlayBGM(const std::string& name, bool loop = true);
	static void StopBGM();
	static void FadeOutBGM(int frame);

	//SE
	static void PlaySE(const std::string& name, bool loop);
	static void StopSE(const std::string& name);
private:
	static std::unordered_map<std::string, int>m_bgmHandles;
	static std::unordered_map<std::string, int>m_seHandles;

	static int m_currentBGM;

	// フェード用
	static int m_fadeFrame;
	static int m_fadeTimer;
};

