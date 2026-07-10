#pragma once

#include <SFML/System/Vector2.hpp>

#include "core/GameObject.h"
#include "core/Types.h"

class Bullet;

// ===========================================================================
// Player：自机。
//
// 操作（默认）：
//   方向键 / WASD     移动
//   Z(或空格)         开火
//   X                 完美格挡（极限距离触发：弹反/摧毁敌弹，获得无敌+变体攻击）
//   Shift             完美闪避（较大距离触发：短暂无敌+子弹时间+变体攻击）
//   C                 使用炸弹（清屏）
//   Esc               暂停
//
// 创新机制核心：FindClosestEnemyBullet 找出最近敌弹，结合 WARNING_RADIUS /
// PARRY_RADIUS 判定是否在“可触发区间”，从而区分完美/普通操作。
// ===========================================================================

class Player : public GameObject {
public:
    Player();
    ~Player() override = default;

    ObjectType GetType() const override { return ObjectType::Player; }

    void OnInit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;
    void OnDestroy() override {}

    // —— 输入（由状态或自机内部调用）——
    void HandleInput();

    // —— 伤害与生命 ——
    void TakeDamage(int amount);
    int  GetHealth() const { return health; }
    int  GetMaxHealth() const { return maxHealth; }
    bool IsInvincible() const { return invincibleTimer > 0.0f; }
    bool IsPlayerDead() const { return health <= 0; }

    // —— 火力 ——
    int  GetPowerLevel() const { return powerLevel; }
    void SetPowerLevel(int lv);
    bool IsVariantAttack() const { return variantTimer > 0.0f; } // 变体攻击激活中

    // —— 完美机制 ——
    float GetParryCooldown() const { return parryCooldown; }
    float GetDodgeCooldown() const { return dodgeCooldown; }
    void  OnPerfectDodge();                 // 闪避成功回调（触发音效/特效/无敌/子弹时间）
    void  OnPerfectParry(Bullet* bullet);   // 格挡成功回调（弹开或摧毁 bullet）

    void  UseBomb();                        // 清屏炸弹

private:
    void  UpdateMovement(float dt);
    void  UpdateTimers(float dt);
    void  UpdateBulletTime(float dt);
    void  Fire();
    void  FireNormal();
    void  FireVariant();
    void  CheckPerfectActions();

    // 返回最近敌弹；outDistance 为距离，无候选返回 nullptr
    Bullet* FindClosestEnemyBullet(float& outDistance) const;

    int   health = 3;
    int   maxHealth = 3;
    int   powerLevel = 1;        // 影响子弹数量/伤害
    int   bombCount = 3;         // 炸弹数

    float fireCooldown = 0.0f;
    float fireInterval = 0.16f;

    // 完美机制计时器（> 0 表示激活中）
    float invincibleTimer = 0.0f;
    float variantTimer = 0.0f;
    float parryCooldown = 0.0f;
    float dodgeCooldown = 0.0f;
    float bulletTimeTimer = 0.0f;

    sf::Vector2f velocity;
};
