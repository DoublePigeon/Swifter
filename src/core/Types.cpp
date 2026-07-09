#pragma once

#include <string>

// ===========================================================================
// 全局枚举与类型定义
// 这些类型贯穿整个游戏，统一放在这里避免循环依赖。
// ===========================================================================

// 阵营：用于伤害判定与子弹归属
enum class Faction {
    Player,
    Enemy,
    Neutral
};

// 对象类型标签：供 ObjectManager 快速查询分类
enum class ObjectType {
    Player,
    Enemy,
    Bullet,
    Item,
    Effect
};

// 敌机种类（用于关卡文件解析与生成）
enum class EnemyType {
    Normal,   // 普通直线下落敌机
    Shooter,  // 会发射子弹的敌机
    Boss      // BOSS
};

// 道具种类
enum class ItemType {
    None,        // 不掉落
    Heal,        // 回血
    PowerUp,     // 火力升级
    Bomb,        // 全屏清弹
    ScoreBonus,  // 加分
    Shield       // 护盾
};

// 弹幕发射模式（BOSS / 射手敌机使用）
enum class BulletPattern {
    SingleAimed,  // 单发瞄准自机
    Spread,       // 扇形散射
    Circle,       // 环形全向
    Spiral,       // 螺旋
    RandomSpray,  // 随机喷射
    Wave,         // 波浪
    Laser         // 激光（直线高速）
};

// 游戏状态机标识
enum class StateID {
    Menu,
    LevelSelect,
    Play,
    Pause,
    GameOver,
    Victory
};
