#include "ResourceManager.h"

namespace
{
    constexpr const wchar_t* kPlayerModelPath = L"data/model/player/Player.mv1";
    constexpr const wchar_t* kStageModelPath = L"data/model/stage/stage.mv1";
    constexpr const wchar_t* kCrabEnemyModelPath = L"data/model/enemy/Crab.mv1";
    constexpr const wchar_t* kHatModelPath = L"data/model/hat/hat.mv1";
    constexpr const wchar_t* kStarModelPath = L"data/model/item/Star.mv1";

    constexpr const wchar_t* kSkyFrontPath = L"data/graph/skybox/sky_front.png";
    constexpr const wchar_t* kSkyRightPath = L"data/graph/skybox/sky_right.png";
    constexpr const wchar_t* kSkyBackPath = L"data/graph/skybox/sky_back.png";
    constexpr const wchar_t* kSkyLeftPath = L"data/graph/skybox/sky_left.png";
    constexpr const wchar_t* kSkyUpPath = L"data/graph/skybox/sky_up.png";
    constexpr const wchar_t* kSkyBottomPath = L"data/graph/skybox/sky_bottom.png";
}


ResourceManager::ResourceManager():
    m_playerModelHandle(-1),
    m_stageModelHandle(-1),
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
    //ステージ
    m_stageModelHandle = MV1LoadModel(kStageModelPath);
    m_modelHandleTable[ModelType::stage] = { m_stageModelHandle };
	//カニ敵
	m_crabEnemyModelHandle = MV1LoadModel(kCrabEnemyModelPath);
	m_modelHandleTable[ModelType::crabEnemy] = { m_crabEnemyModelHandle };
    //帽子
    m_hatModelHandle = MV1LoadModel(kHatModelPath);
    m_modelHandleTable[ModelType::hat] = { m_hatModelHandle };
    //スター
 //   m_starModelHadle = MV1LoadModel(kStarModelPath);
  //  m_modelHandleTable[ModelType::star] = { m_starModelHadle };
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
