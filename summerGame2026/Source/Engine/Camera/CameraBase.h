#pragma once
#include "Engine/Core/GameObject.h"
#include"Engine/Math/Transform.h"

enum class CameraName
{
	follow,
	lookat,
};

class CameraBase : public GameObject
{
public:
	virtual ~CameraBase() = default;
	//初期化
	virtual void Init() abstract override;
	//終了時処理
	virtual void End()abstract override;
	//更新
	virtual void Update()abstract override;
	//Drawはいらないので空実装
	virtual void Draw()override {};
	//DxLibへの反映
	virtual void Apply()abstract;
	//自身のCameraNameを返す
	virtual CameraName GetCameraName()abstract;
	//DxLibに反映するかどうかをセット
	void SetIsApply(bool is);
	//DxLibに反映するかどうか
	bool GetIsApply();
	/// <summary>
	/// カメラのTransformを取得する
	/// </summary>
	/// <returns>Transform</returns>
	Transform& GetTransform();
	/// <summary>
	/// カメラのTransformを取得する(読み取り専用)
	/// </summary>
	/// <returns>Transform</returns>
	const Transform& GetTransform()const;

protected:
	//DxLibに反映するかどうか
	bool m_isApply = 0;
};

