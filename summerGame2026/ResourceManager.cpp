#include "ResourceManager.h"

namespace
{
    const wchar_t* kPlayerModelPath = L"data/model/Player.mv1";
    const wchar_t* kStageModelPath = L"data/Untitled.mv1";
}


ResourceManager::ResourceManager():
    m_playerModelHandle(-1),
    m_stageModelHandle(-1)
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
