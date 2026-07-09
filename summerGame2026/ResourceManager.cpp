#include "ResourceManager.h"

namespace
{
    constexpr const wchar_t* kPlayerModelPath = L"data/model/player/Player.mv1";
    constexpr const wchar_t* kStageModelPath = L"data/model/stage/test2.mv1";
    constexpr const wchar_t* kCrabEnemyModelPath = L"data/model/enemy/Crab.mv1";
    constexpr const wchar_t* kHatModelPath = L"data/model/hat/hat.mv1";
}


ResourceManager::ResourceManager():
    m_playerModelHandle(-1),
    m_stageModelHandle(-1),
	m_crabEnemyModelHandle(-1),
    m_hatModelHandle(-1)
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
}

void ResourceManager::ReleaseResources()
{
    for (const auto all : m_modelHandleTable)
    {
        MV1DeleteModel(all.second);
    }
}

int ResourceManager::GetModel(ModelType type) const
{
    for (const auto all : m_modelHandleTable)
    {
        if (all.first == type)
        {
            return all.second;
        }
    }
    return -1;
}
