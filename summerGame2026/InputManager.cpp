#include "InputManager.h"

namespace
{
	//GetJoypadXInputStateで取得した値を-1.0～1.0の範囲に変換するために使う
	constexpr float kStickInitializeNum = 32767.0f;
	//スティックのデッドゾーン
	constexpr float kDeadZone = 0.2f;
}

InputManager::InputManager() :
	m_inputData{},
	m_lastInputData{},
	m_inputTable{},
	m_stick()
{
	//イベント名を添え字にして、
	// 右辺に実際の入力種別と入力コードの配列を置く

	m_inputTable["left"] = { {PeripheralType::keyBoard,KEY_INPUT_LEFT},
							{PeripheralType::pad1,PAD_INPUT_LEFT} };
	m_inputTable["up"] = { {PeripheralType::keyBoard,KEY_INPUT_UP},
							{PeripheralType::pad1,PAD_INPUT_UP} };
	m_inputTable["right"] = { {PeripheralType::keyBoard,KEY_INPUT_RIGHT},
							{PeripheralType::pad1,PAD_INPUT_RIGHT} };
	m_inputTable["down"] = { {PeripheralType::keyBoard,KEY_INPUT_DOWN},
							{PeripheralType::pad1,PAD_INPUT_DOWN} };

	m_inputTable["A"] = { {PeripheralType::keyBoard,KEY_INPUT_Z},
							{PeripheralType::pad1,PAD_INPUT_A} };
	m_inputTable["B"] = { {PeripheralType::keyBoard,KEY_INPUT_X},
							{PeripheralType::pad1,PAD_INPUT_B} };
	m_inputTable["X"] = { {PeripheralType::keyBoard,KEY_INPUT_C},
							{PeripheralType::pad1,PAD_INPUT_X} };
	m_inputTable["Y"] = { {PeripheralType::keyBoard,KEY_INPUT_V},
							{PeripheralType::pad1,PAD_INPUT_Y} };
	m_inputTable["START"] = { {PeripheralType::keyBoard,KEY_INPUT_RETURN},
							{PeripheralType::pad1,PAD_INPUT_R} };
	m_inputTable["SELECT"] = { {PeripheralType::keyBoard,KEY_INPUT_Z},
							{PeripheralType::pad1,PAD_INPUT_L} };

	//あらかじめ入力データの枠を開けておく
	//ここで枠を開けておかないと、
	//チェックの際にAt関数でクラッシュする可能性がある
	for (const auto& inputInfo : m_inputTable)
	{
		m_inputData[inputInfo.first] = false;
		m_lastInputData[inputInfo.first] = false;
	}
}


void InputManager::Update()
{
	char keyState[256];
	GetHitKeyStateAll(keyState);//生のキーボード情報
	int padState = GetJoypadInputState(DX_INPUT_PAD1);//生のパッド情報
	m_lastInputData = m_inputData;//直前のフレームを更新(前のフレーム情報をコピー)
	//スティックの入力を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &m_stick);
	//すべての入力イベントをチェック
	//ここでinputData_が更新される
	//inputTable_を回して各イベントの入力をチェック
	for (const auto inputInfo : m_inputTable)
	{
		auto& input = m_inputData[inputInfo.first];//inputInfo.firstには"ok"等が入っている
		//InputStateのベクタを回す
		for (const auto& state : inputInfo.second)
		{
			//入力種別をチェック
			switch (state.type)
			{
			case PeripheralType::keyBoard://キーボードの場合
				input = keyState[state.id];
				break;
			case PeripheralType::pad1:
				input = (padState & state.id);
				break;
			}
			//ここでbreakしないと最後のチェックで押されていないとfalseになる
			if (input)
			{
				break;
			}
		}
	}
}

bool InputManager::IsPressed(const char* name) const
{
	return m_inputData.at(name);
}

bool InputManager::IsTriggered(const char* name) const
{
	return m_inputData.at(name) && !m_lastInputData.at(name);
}

Vector2 InputManager::GetLeftStick() const
{
	float x = m_stick.ThumbLX / 32767.0f;
	float y = m_stick.ThumbLY / 32767.0f;

	Vector2 stick{ x,y };

#ifdef _DEBUG
	DrawFormatString(16, 64, 0xffffff, L"Input : LeftStick : %f,%f", x, y);
	DrawFormatString(16, 80, 0xffffff, L"      : LeftStick : %f,%f", m_stick.ThumbLX, m_stick.ThumbLY);
#endif // _DEBUG

	//デッドゾーン処理
	float len = sqrtf(x * x + y * y);
	if (len < kDeadZone)
	{
		return { 0.0f,0.0f };
	}
	//正規化
	if (len > 1.0f)
	{
		len = 1.0f;
	}
	stick.x /= len;
	stick.y /= len;

	return stick;
}

Vector2 InputManager::GetRightStick() const
{
	float x = m_stick.ThumbRX / 32767.0f;
	float y = m_stick.ThumbRY / 32767.0f;

	Vector2 stick{ x,y };

	//デッドゾーン処理
	float len = sqrtf(x * x + y * y);
	if (len < kDeadZone)
	{
		return { 0.0f,0.0f };
	}
	//正規化
	if (len > 1.0f)
	{
		len = 1.0f;
	}
	stick.x /= len;
	stick.y /= len;

	return stick;
}

bool InputManager::AnyPressButtonInput() const
{
	for (const auto& input : m_inputData)
	{
		if (input.first == "A" || input.first == "B" ||
			input.first == "X" || input.first == "Y" ||
			input.first == "START" || input.first == "SELECT")
		{
			if (input.second)
			{
				return true;
			}
		}
	}
	return false;
}