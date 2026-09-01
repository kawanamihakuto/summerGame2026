#include "EffectManager.h"
#include <EffekseerForDxLib.h>

namespace
{
    constexpr int kEffectParticleLimit = 20000;
}

std::map<std::string, int>EffectManager::m_effects;

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
}

void EffectManager::Init()
{
    // DirectX11を使用するようにする
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    // 引数には画面に表示する最大パーティクル数を設定する
    if (Effkseer_Init(kEffectParticleLimit) == -1) { DxLib_End(); }
    // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
    // DXライブラリのデバイスロストした時のコールバックを設定する
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();
    // Zバッファを有効にする
    SetUseZBuffer3D(TRUE);
    // Zバッファへの書き込みを有効にする
    SetWriteZBuffer3D(TRUE);
}

void EffectManager::End()
{
    for (auto& e : m_effects)
    {
        DeleteEffekseerEffect(e.second);
    }

    m_effects.clear();

    Effkseer_End();
}

void EffectManager::Load(const std::string& name, const wchar_t* path)
{
    int handle = LoadEffekseerEffect(path);

    if (handle != -1)
    {
        m_effects[name] = handle;
    }
}

int EffectManager::Play(const std::string& name, Vector3 pos, float yaw)
{
    auto it = m_effects.find(name);
    if (it == m_effects.end())
    {
        return -1;
    }

    int handle = it->second;

    int playHandle = PlayEffekseer3DEffect(handle);

    SetPosPlayingEffekseer3DEffect(playHandle, pos.x, pos.y, pos.z);
    SetRotationPlayingEffekseer3DEffect(playHandle, 0.0f, yaw + DX_PI_F / 2, 0.0f);
    SetScalePlayingEffekseer3DEffect(playHandle, 10.0f, 10.0f, 10.0f);

    return playHandle;
}

void EffectManager::Update()
{
    Effekseer_Sync3DSetting();
    UpdateEffekseer3D();
}

void EffectManager::Draw()
{
    DrawEffekseer3D();
}

void EffectManager::StopAll()
{
    for (auto e : m_effects)
    {
        StopEffekseer3DEffect(e.second);
    }
}
