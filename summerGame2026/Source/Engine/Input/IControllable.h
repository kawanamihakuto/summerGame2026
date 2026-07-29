#pragma once
#include"Engine/Math/Vector2.h"
#include"InputInfo.h"

/// <summary>
/// 操作することができるオブジェクトの抽象基底クラス
/// </summary>
class IControllable
{
public:
	virtual ~IControllable() = default;
	//入力をセットする
	void SetInputInfo(const InputInfo& input) { m_input = input; }
	//入力を取得する
	InputInfo GetInputInfo()const { return m_input; }
	//移動
	virtual void Move() abstract;
	//ジャンプ
	virtual void Jump()abstract;
	//上昇を切る
	virtual void JumpEnd()abstract;

	//操作対象になったときに呼ばれる
	virtual void Controll()abstract;
	//操作対象から外れたときに呼ばれる
	virtual void ExitControll()abstract;
protected:
	InputInfo m_input;
};

