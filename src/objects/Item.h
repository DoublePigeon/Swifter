#pragma once

#include <SFML/System/Vector2.hpp>

#include "core/GameObject.h"
#include "core/Types.h"

class Player;

// ===========================================================================
// Item：掉落道具。
// 缓慢下落并做上下浮动动画；被自机拾取后对自机生效，超时自动消失。
// 效果：回血 / 火力升级 / 炸弹 / 护盾 / 加分。
// ===========================================================================

class Item : public GameObject {
public:
    Item();
    ~Item() override = default;

    ObjectType GetType() const override { return ObjectType::Item; }

    void OnInit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;
    void OnDestroy() override {}

    void     SetItemType(ItemType t) { type = t; }
    ItemType GetItemType() const { return type; }

    // 对玩家生效（由碰撞管理器在拾取时调用）
    void ApplyToPlayer(Player* player);

private:
    ItemType type = ItemType::Heal;
    sf::Vector2f velocity;
    float bobTimer = 0.0f;    // 浮动动画相位
    float lifeTime = 0.0f;
    float maxLifeTime = 12.0f;
};
