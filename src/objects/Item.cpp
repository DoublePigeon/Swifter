#include "objects/Item.h"
#include "objects/Player.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "managers/ScoreManager.h"

#include <SFML/Graphics/Texture.hpp>

#include <cmath>

// ===========================================================================
// Item：掉落道具。
// ===========================================================================

Item::Item() {
    radius = 14.0f;
    maxLifeTime = 12.0f;
}

void Item::OnInit() {
    // 根据类型设置纹理
    switch (type) {
        case ItemType::Heal:
            sprite.setTexture(context->resources->GetTexture("assets/images/item_heal.png"), true);
            break;
        case ItemType::Bomb:
            sprite.setTexture(context->resources->GetTexture("assets/images/item_bomb.png"), true);
            break;
        case ItemType::PowerUp:
            // PowerUp 使用 heal 纹理但染色
            sprite.setTexture(context->resources->GetTexture("assets/images/item_heal.png"), true);
            sprite.setColor(sf::Color(255, 200, 50));
            break;
        case ItemType::ScoreBonus:
            sprite.setTexture(context->resources->GetTexture("assets/images/item_bomb.png"), true);
            sprite.setColor(sf::Color(255, 255, 100));
            break;
        case ItemType::Shield:
            sprite.setTexture(context->resources->GetTexture("assets/images/item_heal.png"), true);
            sprite.setColor(sf::Color(100, 150, 255));
            break;
        default:
            sprite.setTexture(context->resources->GetTexture("assets/images/dummy.png"), true);
            break;
    }

    auto texSize = sprite.getTexture().getSize();
    sprite.setOrigin({static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f});
    float scale = config::SPRITE_ITEM_W / static_cast<float>(texSize.x);
    sprite.setScale({scale, scale});

    // 缓慢下落
    velocity = {0.0f, 60.0f};
}

void Item::OnUpdate(float dt) {
    if (!active) return;

    lifeTime += dt;
    if (lifeTime >= maxLifeTime) {
        Destroy();
        return;
    }

    // 浮动动画
    bobTimer += dt;
    float bobOffset = std::sin(bobTimer * 3.0f) * 6.0f;

    // 下落
    position += velocity * dt;

    // 出界销毁
    if (position.y > config::WINDOW_HEIGHT + 30.0f) {
        Destroy();
        return;
    }

    sprite.setPosition({position.x, position.y + bobOffset});
    sprite.setRotation(sf::degrees(rotation));
}

void Item::OnRender(sf::RenderTarget& target) {
    if (!active) return;

    // 快过期时闪烁
    if (lifeTime > maxLifeTime - 3.0f) {
        if (static_cast<int>(lifeTime * 8.0f) % 2 == 0) {
            target.draw(sprite);
        }
    } else {
        target.draw(sprite);
    }
}

void Item::ApplyToPlayer(Player* player) {
    if (!player) return;

    switch (type) {
        case ItemType::Heal:
            // 回一点血
            if (player->GetHealth() < player->GetMaxHealth()) {
                player->TakeDamage(-1); // 负伤害即回血
            }
            break;
        case ItemType::PowerUp:
            // 火力升级
            player->SetPowerLevel(player->GetPowerLevel() + 1);
            break;
        case ItemType::Bomb:
            // 加一个炸弹
            player->UseBomb(); // 直接使用炸弹效果
            break;
        case ItemType::ScoreBonus:
            // 加分
            if (context && context->score) {
                context->score->AddScore(500);
            }
            break;
        case ItemType::Shield:
            // 短暂无敌
            player->TakeDamage(0); // 临时无敌效果通过 OnPerfectDodge 获得
            // 通过变体攻击触发无敌
            {
                // 触发短暂无敌（模拟护盾）
                player->OnPerfectDodge();
            }
            break;
        default:
            break;
    }
}

