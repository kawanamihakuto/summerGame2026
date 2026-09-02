#include "ResourceManager.h"

namespace
{
    constexpr const wchar_t* kPlayerModelPath = L"data/model/player/Player2.mv1";
    constexpr const wchar_t* kStageModelPath = L"data/model/stage/stage.mv1";
    constexpr const wchar_t* kStageAppearanceModelPath = L"data/model/stage/stageAppearance.mv1";
    constexpr const wchar_t* kCrabEnemyModelPath = L"data/model/enemy/Crab.mv1";
    constexpr const wchar_t* kHatModelPath = L"data/model/hat/hat.mv1";
    constexpr const wchar_t* kStarModelPath = L"data/model/item/Star.mv1";

    constexpr const wchar_t* kSkyFrontPath = L"data/graph/skybox/sky_front.png";
    constexpr const wchar_t* kSkyRightPath = L"data/graph/skybox/sky_right.png";
    constexpr const wchar_t* kSkyBackPath = L"data/graph/skybox/sky_back.png";
    constexpr const wchar_t* kSkyLeftPath = L"data/graph/skybox/sky_left.png";
    constexpr const wchar_t* kSkyUpPath = L"data/graph/skybox/sky_up.png";
    constexpr const wchar_t* kSkyBottomPath = L"data/graph/skybox/sky_bottom.png";

	constexpr const wchar_t* kTitleGraphPath = L"data/graph/title/title.png";
	constexpr const wchar_t* kPressAnyButtonGraphPath = L"data/graph/title/pressAnyButton.png";
	constexpr const wchar_t* kClearGraphPath = L"data/graph/clear/clear.png";
    
	constexpr const wchar_t* kWaitSpawnEffectPath = L"data/effect/spawn.efk";
    constexpr const wchar_t* kSpawnEffectPath = L"data/effect/spawn2.efk";
    constexpr const wchar_t* kJumpEffectPath = L"data/effect/jump.efk";
    constexpr const wchar_t* kGetStarEffectPath = L"data/effect/getStar.efk";
	constexpr const wchar_t* kEnemyDeathEffectPath = L"data/effect/enemyDeath.efk";
}


ResourceManager::ResourceManager():
    m_playerModelHandle(-1),
    m_stageColliderModelHandle(-1),
	m_crabEnemyModelHandle(-1),
    m_hatModelHandle(-1),
    m_starModelHadle(-1),
    m_skyBoxGraphHandles({})
{
}


ResourceManager::~ResourceManager()
{
}

ResourceManager& ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadResources()
{
    //プレイヤー
    m_playerModelHandle = MV1LoadModel(kPlayerModelPath); 
    m_modelHandleTable[ModelType::player] = { m_playerModelHandle };
    //ステージ当たり判定
    m_stageColliderModelHandle = MV1LoadModel(kStageModelPath);
    m_modelHandleTable[ModelType::stageCollider] = { m_stageColliderModelHandle };
	//ステージ見た目
    m_stageAppearanceModelHandle = MV1LoadModel(kStageAppearanceModelPath);
    m_modelHandleTable[ModelType::stageAppearance] = { m_stageAppearanceModelHandle };
    //カニ敵
	m_crabEnemyModelHandle = MV1LoadModel(kCrabEnemyModelPath);
	m_modelHandleTable[ModelType::crabEnemy] = { m_crabEnemyModelHandle };
    //帽子
    m_hatModelHandle = MV1LoadModel(kHatModelPath);
    m_modelHandleTable[ModelType::hat] = { m_hatModelHandle };
    //スター
    m_starModelHadle = MV1LoadModel(kStarModelPath);
    m_modelHandleTable[ModelType::star] = { m_starModelHadle };

    //スカイボックス(前)
    m_skyBoxGraphHandles[0] = LoadGraph(kSkyFrontPath);
    m_graphHandleTable[GraphType::skyFront] = { m_skyBoxGraphHandles[0] };
    //スカイボックス(右)
    m_skyBoxGraphHandles[1] = LoadGraph(kSkyRightPath);
    m_graphHandleTable[GraphType::skyRight] = { m_skyBoxGraphHandles[1] };
    //スカイボックス(後)
    m_skyBoxGraphHandles[2] = LoadGraph(kSkyBackPath);
    m_graphHandleTable[GraphType::skyBack] = { m_skyBoxGraphHandles[2] };
    //スカイボックス(左)
    m_skyBoxGraphHandles[3] = LoadGraph(kSkyLeftPath);
    m_graphHandleTable[GraphType::skyLeft] = { m_skyBoxGraphHandles[3] };
    //スカイボックス(上)
    m_skyBoxGraphHandles[4] = LoadGraph(kSkyUpPath);
    m_graphHandleTable[GraphType::skyUp] = { m_skyBoxGraphHandles[4] };
    //スカイボックス(下)
    m_skyBoxGraphHandles[5] = LoadGraph(kSkyBottomPath);
    m_graphHandleTable[GraphType::skyBottom] = { m_skyBoxGraphHandles[5] };

    //タイトルグラフィック
    m_graphHandleTable[GraphType::title] = { LoadGraph(kTitleGraphPath) };
	//プレスエニーボタン
	m_graphHandleTable[GraphType::pressAnyButton] = { LoadGraph(kPressAnyButtonGraphPath) };
    //クリアグラフィック
    m_graphHandleTable[GraphType::clear] = { LoadGraph(kClearGraphPath) };

	//スポーンエフェクト
	m_effectHandleTable[EffectType::waitSpawn] = { kWaitSpawnEffectPath };
	m_effectHandleTable[EffectType::spawn] = { kSpawnEffectPath };
    //ジャンプエフェクト
	m_effectHandleTable[EffectType::jump] = { kJumpEffectPath };
	//スター取得エフェクト
	m_effectHandleTable[EffectType::getStar] = { kGetStarEffectPath };
	//敵死亡エフェクト
	m_effectHandleTable[EffectType::enemyDeath] = { kEnemyDeathEffectPath };
}

void ResourceManager::ReleaseResources()
{
    //モデル全部解放
    for (const auto all : m_modelHandleTable)
    {
        MV1DeleteModel(all.second);
    }

    //画像全部開放
    for (const auto all : m_graphHandleTable)
    {
        DeleteGraph(all.second);
    }
}

int ResourceManager::GetModel(ModelType type) const
{
    //モデル取得
    for (const auto all : m_modelHandleTable)
    {
        if (all.first == type)
        {
            return all.second;
        }
    }
    return -1;
}

int ResourceManager::GetGraph(GraphType type) const
{
    //画像グラフ
    for (const auto all : m_graphHandleTable)
    {
        if (all.first == type)
        {
            return all.second;
        }
    }
    return -1;
}

const wchar_t* ResourceManager::GetEffectPath(EffectType type) const
{
    //エフェクト
    for (const auto all : m_effectHandleTable)
    {
        if (all.first == type)
        {
            return all.second;
        }
    }
    return nullptr;
}
