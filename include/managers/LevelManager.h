#pragma once

#include <queue>
#include <string>

#include "data/LevelData.h"
#include "core/Types.h"

class ObjectManager;
struct GameContext;

// ===========================================================================
// LevelManager：按时间轴驱动敌机生成。
// LoadLevel 读取关卡文件并解析为按时间排序的生成事件队列；
// Update 累加时间，到点即把事件交给 ObjectManager 生成对应敌机。
// 当队列清空且场上无敌机（BOSS 已被击杀）时关卡完成。
// ===========================================================================

class LevelManager {
public:
    LevelManager();
    void SetContext(GameContext* ctx);

    bool  LoadLevel(const std::string& filename); // 解析关卡文件
    void  Start();                                 // 重置计时器
    void  Update(float dt);                        // 驱动生成

    bool  IsLevelComplete() const; // 队列空且场上无敌机
    bool  IsBossDefeated() const;  // 关卡内 BOSS 是否被击杀
    float GetLevelTime() const { return levelTimer; }

    const std::string& GetLevelName() const { return levelName; }
    void  SetLevelName(const std::string& name) { levelName = name; }

private:
    GameContext* context = nullptr;
    float levelTimer = 0.0f;
    std::queue<EnemySpawnEvent> spawnQueue;
    std::string levelName;
    bool levelComplete = false;
};
