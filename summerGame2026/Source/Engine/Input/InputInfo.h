#pragma once
#include"Engine/Math/Vector2.h"

/// <summary>
/// キャラクターに渡す入力
/// </summary>
struct InputInfo
{
	//左スティックの入力値
	Vector2 move = {};
	
	//ジャンプボタンが今押されたか
	bool isJumpDown = false;
	//ジャンプボタンが今離されたか
	bool isJumpUp = false;
};