#pragma once
#include"Precompiled.h"

class GameObject;
class GameObjectManager
{
public:
    /// <summary>
    /// オブジェクトを追加する
    /// </summary>
    /// <param name="object">追加するオブジェクト</param>
    void Add(std::unique_ptr<GameObject> object);
    /// <summary>
    /// オブジェクトすべての初期化
    /// </summary>
    void Init();
    /// <summary>
    /// オブジェクトすべての更新
    /// </summary>
    void Update();
    /// <summary>
    /// オブジェクトすべての描画
    /// </summary>
    void Draw();
    /// <summary>
    /// オブジェクトすべての解放
    /// </summary>
    void Clear();

private:
    //オブジェクト配列
    std::vector<std::unique_ptr<GameObject>> m_objects;
};