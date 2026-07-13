#pragma once

// ===========================================================================
// 全局常量配置
// 把所有“魔法数字”集中管理，方便调参。
// ===========================================================================

namespace config {
    // ---- 窗口 ----
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr const char* WINDOW_TITLE = "Swifter";

    // ---- 玩家自机 ----
    constexpr float PLAYER_SPEED = 360.0f;
    constexpr int   PLAYER_MAX_HEALTH = 3;
    constexpr float PLAYER_RADIUS = 10.0f;          // 判定点
    constexpr float PLAYER_FIRE_INTERVAL = 0.16f;
    constexpr float PLAYER_BULLET_SPEED = 600.0f;

    // ---- 完美格挡 / 完美闪避 ----
    // 当敌弹/敌机进入“危险区间”时按下对应键即可触发。
    constexpr float DODGE_WARNING_RADIUS = 130.0f;  // 闪避可触发半径（较大）
    constexpr float PARRY_RADIUS = 55.0f;           // 格挡可触发半径（更极限）
    constexpr float DODGE_INVINCIBLE_TIME = 1.5f;   // 闪避成功无敌时长
    constexpr float PARRY_INVINCIBLE_TIME = 1.0f;   // 格挡成功无敌时长
    constexpr float VARIANT_ATTACK_TIME = 5.0f;     // 变体攻击持续时长
    constexpr float DODGE_COOLDOWN = 0.9f;          // 闪避冷却
    constexpr float PARRY_COOLDOWN = 1.4f;          // 格挡冷却
    constexpr float BULLET_TIME_DURATION = 1.2f;    // 闪避后子弹时间持续
    constexpr float BULLET_TIME_SCALE = 0.35f;      // 子弹时间敌方子弹减速比

    // ---- 敌方子弹 ----
    constexpr float ENEMY_BULLET_SPEED = 220.0f;

    // ---- 普通敌机 ----
    constexpr float NORMAL_ENEMY_SPEED = 140.0f;

    // ---- 精灵显示尺寸（贴图分辨率很大，需要缩放）----
    constexpr float SPRITE_PLAYER_W = 48.0f;       // 自机显示宽度
    constexpr float SPRITE_BULLET_SMALL_W = 14.0f; // 小子弹显示宽度
    constexpr float SPRITE_BULLET_BIG_W = 24.0f;   // 大子弹显示宽度
    constexpr float SPRITE_ENEMY_W = 44.0f;        // 普通敌机显示宽度
    constexpr float SPRITE_BOSS_W = 80.0f;         // Boss 显示宽度
    constexpr float SPRITE_ITEM_W = 30.0f;         // 道具显示宽度
    constexpr float SPRITE_EFFECT_W = 60.0f;       // 特效显示宽度

    // ---- 文件路径 ----
    constexpr const char* LEVELS_DIR = "assets/levels/";
    constexpr const char* LEVEL_INDEX = "assets/levels/index.txt";
    constexpr const char* SCORE_FILE = "score.dat";
} // namespace config
