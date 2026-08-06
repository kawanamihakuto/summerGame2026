#pragma once
#include"Engine/Core/Precompiled.h"

/// <summary>
/// モデルの種類
/// </summary>
enum class ModelType
{
	player,
	stage,
	crabEnemy,
	hat,
	star
};
/// <summary>
/// グラフィックの種類
/// </summary>
enum class GraphType
{
	skyFront,
	skyRight,
	skyBack,
	skyLeft,
	skyUp,
	skyBottom,
};

class ResourceManager
{
private:
	//newも変数宣言もできなくする
	ResourceManager();
	//コピーコンストラクタも無効にする
	ResourceManager(const ResourceManager& app) = delete;
	//代入も無効にする
	void operator = (const ResourceManager& app) = delete;

public:
	~ResourceManager();

	static ResourceManager& GetInstance();
	//リソースのロード
	void LoadResources();
	//リソースの開放
	void ReleaseResources();
	//指定したモデルハンドルを取得する
	int GetModel(ModelType type)const;
	//指定した画像のハンドルを取得する
	int GetGraph(GraphType type)const;

private:
	//モデルの種類と実際のハンドル
	std::map<ModelType, int>m_modelHandleTable;
	//画像の種類と実際のハンドル
	std::map<GraphType, int>m_graphHandleTable;

	//プレイヤーモデル
	int m_playerModelHandle;
	//ステージモデル
	int m_stageModelHandle;
	//かにエネミーモデル
	int m_crabEnemyModelHandle;
	//帽子モデル
	int m_hatModelHandle;
	//スターモデル
	int m_starModelHadle;
	//スカイボックスグラフィック
	int m_skyBoxGraphHandles[6];
};

