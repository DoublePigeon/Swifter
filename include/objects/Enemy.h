#pragma once

#include <SFML/System/Vector2.hpp>

#include "core/GameObject.h"
#include "core/Types.h"

// ===========================================================================
// Enemy：所有敌机基类。
// 子类实现 UpdateBehavior（移动）与 Fire（射击）。
// 被击杀时按 dropItem 掉落道具，并按 scoreValue 给分。
// ===========================================================================

class Enemy : public GameObject {
public:
    Enemy();
    ~Enemy() override = default;

    ObjectType GetType() const override { return ObjectType::Enemy; }

    void OnInit() override;
    void OnUpdate(float dt) override; // 调用子类 UpdateBehavior + Fire 计时
    void OnRender(sf::RenderTarget& target) override;
    void OnDestroy() override {}

    // 子类实现
    virtual void UpdateBehavior(float dt) {}
    virtual void Fire() {}

    // —— 生命 ——
    void TakeDamage(int amount);
    int  GetHealth() const { return health; }
    int  GetMaxHealth() const { return maxHealth; }
    bool IsAlive() const { return health > 0; }

    // —— 掉落与给分 ——
    void     SetDropItem(ItemType type) { dropItem = type; }
    ItemType GetDropItem() const { return dropItem; }
    int      GetScoreValue() const { return scoreValue; }
    void     SetScoreValue(int v) { scoreValue = v; }

    // 敌机种类标识
    virtual EnemyType GetEnemyType() const = 0;

protected:
    int   health = 1;
    int   maxHealth = 1;
    int   scoreValue = 100;
    ItemType dropItem = ItemType::None;

    float fireTimer = 0.0f;
    float fireInterval = 2.0f;
    sf::Vector2f velocity;
};
