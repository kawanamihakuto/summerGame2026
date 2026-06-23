#pragma once
#include"Engine/Math/Transform.h"
class GameObject
{
public:
	virtual ~GameObject() = default;
	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// 終了時処理関数
	/// </summary>
	virtual void End()abstract;
	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update()abstract;
	/// <summary>
	/// 描画関数
	/// </summary>
	virtual void Draw()abstract;

protected:
	TransForm m_transform;
};

