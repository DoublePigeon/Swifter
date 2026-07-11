#include "objects/NormalEnemy.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"

#include <SFML/Graphics/Texture.hpp>

// ===========================================================================
// NormalEnemy：普通直线下落敌机，不射击，碰到自机造成伤害。
// ===========================================================================

NormalEnemy::NormalEnemy() {
    health = 1;
    maxHealth = 1;
    scoreValue = 100;
    dropItem = ItemType::None;
    radius = 18.0f;
}

void NormalEnemy::OnInit() {
    sprite.setTexture(context->resources->GetTexture("assets/images/enemy1.png"));
    auto texSize = sprite.getTexture().getSize();
    sprite.setOrigin({static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f});

    velocity = sf::Vector2f(0.0f, config::NORMAL_ENEMY_SPEED);
}

void NormalEnemy::UpdateBehavior(float dt) {
    // 直线下落
    position += velocity * dt;
}

