#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>

#include "core/Types.h"

// ===========================================================================
// 关卡数据结构
// ===========================================================================

// 单条敌机生成事件
struct EnemySpawnEvent {
    float          spawnTime = 0.0f;   // 关卡开始后多少秒生成
    EnemyType      type = EnemyType::Normal;
    sf::Vector2f   position;           // 出生坐标
    ItemType       dropItem = ItemType::None;
    std::string    pattern;            // 可选：弹幕模式名（BOSS/射手用）
};

// 关卡列表中的一项
struct LevelInfo {
    std::string filename;     // 相对 assets/levels/ 的文件名
    std::string displayName;  // 显示名
    int         index = 0;
    bool        unlocked = false;
};
