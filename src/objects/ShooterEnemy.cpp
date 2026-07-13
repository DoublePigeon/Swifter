#include "objects/ShooterEnemy.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "core/Math.h"
#include "managers/ObjectManager.h"
#include "objects/Player.h"
#include "objects/Bullet.h"

#include <SFML/Graphics/Texture.hpp>

// ===========================================================================
// ShooterEnemy：射手型敌机，下落到某高度后悬停并周期性向自机发射子弹。
// ===========================================================================

ShooterEnemy::ShooterEnemy() {
    health = 2;
    maxHealth = 2;
    scoreValue = 200;
    dropItem = ItemType::PowerUp;
    radius = 20.0f;
    fireInterval = 1.8f;
    hoverY = 120.0f;
}

void ShooterEnemy::OnInit() {
    sprite.setTexture(context->resources->GetTexture("assets/images/enemy2.png"), true);
    auto texSize = sprite.getTexture().getSize();
    sprite.setOrigin({static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f});
    float scale = config::SPRITE_ENEMY_W / static_cast<float>(texSize.x);
    sprite.setScale({scale, scale});

    velocity = sf::Vector2f(0.0f, 100.0f);
}

void ShooterEnemy::UpdateBehavior(float dt) {
    if (!hovering) {
        // 下落到悬停高度
        position += velocity * dt;
        if (position.y >= hoverY) {
            position.y = hoverY;
            hovering = true;
        }
    }
    // 悬停后不做额外移动
}

void ShooterEnemy::Fire() {
    if (!context || !context->objects) return;

    auto* player = context->player;
    if (!player || !player->IsActive() || player->IsDead()) return;

    // 朝自机方向发射子弹
    sf::Vector2f toPlayer = player->GetPosition() - position;
    sf::Vector2f dir = math::Normalize(toPlayer);

    context->objects->SpawnBullet(
        position,
        dir * config::ENEMY_BULLET_SPEED,
        Faction::Enemy
    );
}

