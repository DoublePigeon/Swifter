#include "objects/Bullet.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "core/Math.h"
#include "core/Time.h"
#include "managers/ObjectManager.h"
#include "objects/Player.h"

#include <SFML/Graphics/Texture.hpp>

// ===========================================================================
// Bullet：通用子弹（玩家与敌方共用）。
// ===========================================================================

Bullet::Bullet() {
    radius = 6.0f;
}

void Bullet::OnInit() {
    // 根据阵营和变体设置纹理
    if (faction == Faction::Player) {
        if (variant) {
            sprite.setTexture(context->resources->GetTexture("assets/images/big_bullet.png"), true);
            radius = 12.0f;
        } else {
            sprite.setTexture(context->resources->GetTexture("assets/images/small_bullet.png"), true);
            radius = 6.0f;
        }
    } else {
        sprite.setTexture(context->resources->GetTexture("assets/images/small_bullet.png"), true);
        sprite.setColor(sf::Color(255, 100, 100)); // 敌弹偏红
        radius = 6.0f;
    }
    // 设置原点为中心
    auto texSize = sprite.getTexture().getSize();
    sprite.setOrigin({static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f});
    // 缩放
    float targetW = variant ? config::SPRITE_BULLET_BIG_W : config::SPRITE_BULLET_SMALL_W;
    float scale = targetW / static_cast<float>(texSize.x);
    sprite.setScale({scale, scale});
}

void Bullet::OnUpdate(float dt) {
    if (!active) return;

    // 敌方子弹受子弹时间影响
    float effectiveDt = (faction == Faction::Enemy) ? dt * Time::Instance().BulletTimeFactor() : dt;
    lifeTime += effectiveDt;

    // 超时销毁
    if (lifeTime >= maxLifeTime) {
        Destroy();
        return;
    }

    // 追踪弹：朝向玩家
    if (homing && faction == Faction::Enemy && context && context->player) {
        auto* player = context->player;
        if (player->IsActive() && !player->IsDead()) {
            sf::Vector2f toPlayer = player->GetPosition() - position;
            float dist = math::Length(toPlayer);
            if (dist > 1.0f) {
                sf::Vector2f dirToPlayer = toPlayer / dist;
                sf::Vector2f currentDir = math::Normalize(velocity);
                sf::Vector2f newDir = math::Normalize(
                    currentDir + dirToPlayer * homingRate * effectiveDt
                );
                velocity = newDir * speed;
            }
        }
    }

    // 移动
    position += velocity * effectiveDt;

    // 出界检测
    const float margin = 50.0f;
    if (position.x < -margin || position.x > config::WINDOW_WIDTH + margin ||
        position.y < -margin || position.y > config::WINDOW_HEIGHT + margin) {
        Destroy();
        return;
    }

    sprite.setPosition(position);
    sprite.setRotation(sf::degrees(rotation));
}

void Bullet::OnRender(sf::RenderTarget& target) {
    if (!active) return;
    target.draw(sprite);
}

void Bullet::SetHoming(bool h, float rate) {
    homing = h;
    homingRate = rate;
}

void Bullet::Deflect() {
    if (deflected) return;
    deflected = true;

    // 阵营变为玩家方
    faction = Faction::Player;
    // 反转方向并加速
    velocity = -velocity;
    speed = config::PLAYER_BULLET_SPEED;
    velocity = math::Normalize(velocity) * speed;
    // 更新纹理为玩家子弹
    sprite.setTexture(context->resources->GetTexture("assets/images/small_bullet.png"), true);
    sprite.setColor(sf::Color::White);
    auto texSize = sprite.getTexture().getSize();
    float scale = config::SPRITE_BULLET_SMALL_W / static_cast<float>(texSize.x);
    sprite.setScale({scale, scale});
}

