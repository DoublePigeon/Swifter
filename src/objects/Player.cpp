#include "objects/Player.h"
#include "objects/Bullet.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "core/Input.h"
#include "core/Math.h"
#include "core/Time.h"
#include "managers/ObjectManager.h"
#include "managers/UIManager.h"
#include "managers/AudioManager.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include <limits>

// ===========================================================================
// Player：自机。
// ===========================================================================

Player::Player() {
    radius = config::PLAYER_RADIUS;
    health = config::PLAYER_MAX_HEALTH;
    maxHealth = config::PLAYER_MAX_HEALTH;
    powerLevel = 1;
    bombCount = 3;
    fireInterval = config::PLAYER_FIRE_INTERVAL;
}

void Player::OnInit() {
    sprite.setTexture(context->resources->GetTexture("assets/images/self_plane.png"), true);
    auto texSize = sprite.getTexture().getSize();
    sprite.setOrigin({static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f});
    float scale = config::SPRITE_PLAYER_W / static_cast<float>(texSize.x);
    sprite.setScale({scale, scale});
    position = {config::WINDOW_WIDTH / 2.0f, config::WINDOW_HEIGHT - 80.0f};
}

void Player::OnUpdate(float dt) {
    if (!active) return;

    HandleInput();
    UpdateTimers(dt);
    UpdateMovement(dt);
    UpdateBulletTime(dt);

    // 边界限制
    position.x = math::Clamp(position.x, radius, config::WINDOW_WIDTH - radius);
    position.y = math::Clamp(position.y, radius, config::WINDOW_HEIGHT - radius);

    sprite.setPosition(position);
    sprite.setRotation(sf::degrees(rotation));
}

void Player::OnRender(sf::RenderTarget& target) {
    if (!active) return;

    // 无敌时闪烁
    if (IsInvincible()) {
        if (static_cast<int>(invincibleTimer * 10.0f) % 2 == 0) {
            target.draw(sprite);
        }
    } else {
        target.draw(sprite);
    }
}

// ===========================================================================
// 输入处理
// ===========================================================================

void Player::HandleInput() {
    auto& input = Input::Instance();

    // 移动方向
    velocity = {0.0f, 0.0f};
    if (input.IsKeyHeld(sf::Keyboard::Key::Left) || input.IsKeyHeld(sf::Keyboard::Key::A)) {
        velocity.x = -config::PLAYER_SPEED;
    }
    if (input.IsKeyHeld(sf::Keyboard::Key::Right) || input.IsKeyHeld(sf::Keyboard::Key::D)) {
        velocity.x = config::PLAYER_SPEED;
    }
    if (input.IsKeyHeld(sf::Keyboard::Key::Up) || input.IsKeyHeld(sf::Keyboard::Key::W)) {
        velocity.y = -config::PLAYER_SPEED;
    }
    if (input.IsKeyHeld(sf::Keyboard::Key::Down) || input.IsKeyHeld(sf::Keyboard::Key::S)) {
        velocity.y = config::PLAYER_SPEED;
    }

    // 射击（持续按住）
    if (input.IsKeyHeld(sf::Keyboard::Key::Z) || input.IsKeyHeld(sf::Keyboard::Key::Space)) {
        Fire();
    }

    // 完美格挡（本帧刚按下 X）
    if (input.IsKeyPressed(sf::Keyboard::Key::X)) {
        CheckPerfectActions();
    }

    // 完美闪避（本帧刚按下 Shift）
    if (input.IsKeyPressed(sf::Keyboard::Key::LShift) || input.IsKeyPressed(sf::Keyboard::Key::RShift)) {
        CheckPerfectActions();
    }

    // 炸弹（本帧刚按下 C）
    if (input.IsKeyPressed(sf::Keyboard::Key::C)) {
        UseBomb();
    }
}

// ===========================================================================
// 移动
// ===========================================================================

void Player::UpdateMovement(float dt) {
    position += velocity * dt;
}

// ===========================================================================
// 计时器
// ===========================================================================

void Player::UpdateTimers(float dt) {
    if (fireCooldown > 0.0f) fireCooldown -= dt;
    if (invincibleTimer > 0.0f) invincibleTimer -= dt;
    if (variantTimer > 0.0f) variantTimer -= dt;
    if (parryCooldown > 0.0f) parryCooldown -= dt;
    if (dodgeCooldown > 0.0f) dodgeCooldown -= dt;

    if (variantTimer <= 0.0f) {
        variantTimer = 0.0f;
    }
}

void Player::UpdateBulletTime(float dt) {
    if (bulletTimeTimer > 0.0f) {
        bulletTimeTimer -= dt;
        Time::Instance().SetBulletTimeFactor(config::BULLET_TIME_SCALE);
    } else {
        bulletTimeTimer = 0.0f;
        Time::Instance().SetBulletTimeFactor(1.0f);
    }
}

// ===========================================================================
// 射击
// ===========================================================================

void Player::Fire() {
    if (fireCooldown > 0.0f) return;
    fireCooldown = fireInterval;

    if (IsVariantAttack()) {
        FireVariant();
    } else {
        FireNormal();
    }
}

void Player::FireNormal() {
    if (!context || !context->objects) return;

    float bulletSpeed = config::PLAYER_BULLET_SPEED;
    AudioManager::Instance().PlaySfx("shoot_small");

    switch (powerLevel) {
        case 1:
            context->objects->SpawnBullet(
                position + sf::Vector2f(0.0f, -20.0f),
                sf::Vector2f(0.0f, -bulletSpeed),
                Faction::Player
            );
            break;
        case 2:
            context->objects->SpawnBullet(
                position + sf::Vector2f(-8.0f, -16.0f),
                sf::Vector2f(0.0f, -bulletSpeed),
                Faction::Player
            );
            context->objects->SpawnBullet(
                position + sf::Vector2f(8.0f, -16.0f),
                sf::Vector2f(0.0f, -bulletSpeed),
                Faction::Player
            );
            break;
        case 3:
            context->objects->SpawnBullet(
                position + sf::Vector2f(0.0f, -20.0f),
                sf::Vector2f(0.0f, -bulletSpeed),
                Faction::Player
            );
            context->objects->SpawnBullet(
                position + sf::Vector2f(-12.0f, -12.0f),
                sf::Vector2f(-0.15f, -bulletSpeed),
                Faction::Player
            );
            context->objects->SpawnBullet(
                position + sf::Vector2f(12.0f, -12.0f),
                sf::Vector2f(0.15f, -bulletSpeed),
                Faction::Player
            );
            break;
        default: {
            // 4级以上：扇形散射
            int count = powerLevel;
            float spread = 20.0f;
            for (int i = 0; i < count; ++i) {
                float t = (count == 1) ? 0.0f
                    : static_cast<float>(i) / static_cast<float>(count - 1);
                float angle = -90.0f - spread / 2.0f + t * spread;
                sf::Vector2f dir = math::DirectionFromAngle(angle);
                context->objects->SpawnBullet(
                    position + sf::Vector2f(0.0f, -16.0f),
                    dir * bulletSpeed,
                    Faction::Player
                );
            }
            break;
        }
    }
}

void Player::FireVariant() {
    if (!context || !context->objects) return;

    float bulletSpeed = config::PLAYER_BULLET_SPEED * 1.2f;
    AudioManager::Instance().PlaySfx("shoot_big");

    // 变体攻击：更宽散射 + 更大伤害
    int count = 3 + powerLevel * 2;
    float spread = 45.0f;

    for (int i = 0; i < count; ++i) {
        float t = (count == 1) ? 0.0f
            : static_cast<float>(i) / static_cast<float>(count - 1);
        float angle = -90.0f - spread / 2.0f + t * spread;
        sf::Vector2f dir = math::DirectionFromAngle(angle);
        auto* bullet = context->objects->SpawnBullet(
            position + sf::Vector2f(0.0f, -16.0f),
            dir * bulletSpeed,
            Faction::Player,
            true
        );
        if (bullet) {
            bullet->SetDamage(2);
        }
    }
}

// ===========================================================================
// 完美机制
// ===========================================================================

Bullet* Player::FindClosestEnemyBullet(float& outDistance) const {
    if (!context || !context->objects) {
        outDistance = std::numeric_limits<float>::max();
        return nullptr;
    }

    auto bullets = context->objects->GetEnemyBullets();
    Bullet* closest = nullptr;
    float minDistSq = std::numeric_limits<float>::max();

    for (auto* bullet : bullets) {
        if (bullet->IsDead()) continue;
        float distSq = math::DistanceSq(position, bullet->GetPosition());
        if (distSq < minDistSq) {
            minDistSq = distSq;
            closest = bullet;
        }
    }

    outDistance = std::sqrt(minDistSq);
    return closest;
}

void Player::CheckPerfectActions() {
    auto& input = Input::Instance();

    float dist = std::numeric_limits<float>::max();
    Bullet* nearestBullet = FindClosestEnemyBullet(dist);

    // 完美格挡（X 键）：需要在 PARRY_RADIUS 内
    if (input.IsKeyPressed(sf::Keyboard::Key::X)) {
        if (parryCooldown <= 0.0f && nearestBullet && dist <= config::PARRY_RADIUS) {
            OnPerfectParry(nearestBullet);
            return;
        }
    }

    // 完美闪避（Shift 键）：需要在 DODGE_WARNING_RADIUS 内
    if (input.IsKeyPressed(sf::Keyboard::Key::LShift) ||
        input.IsKeyPressed(sf::Keyboard::Key::RShift)) {
        if (dodgeCooldown <= 0.0f && nearestBullet && dist <= config::DODGE_WARNING_RADIUS) {
            OnPerfectDodge();
            return;
        }
    }
}

void Player::OnPerfectDodge() {
    dodgeCooldown = config::DODGE_COOLDOWN;
    invincibleTimer = config::DODGE_INVINCIBLE_TIME;
    variantTimer = config::VARIANT_ATTACK_TIME;
    bulletTimeTimer = config::BULLET_TIME_DURATION;

    if (context && context->objects) {
        context->objects->SpawnEffect(position, "evade", 0.8f);
    }
    AudioManager::Instance().PlaySfx("evade");
    if (context && context->ui) {
        context->ui->ShowMessage("PERFECT DODGE!", 1.0f);
    }
}

void Player::OnPerfectParry(Bullet* bullet) {
    parryCooldown = config::PARRY_COOLDOWN;
    invincibleTimer = config::PARRY_INVINCIBLE_TIME;
    variantTimer = config::VARIANT_ATTACK_TIME;

    if (bullet && !bullet->IsDead()) {
        bullet->Deflect();
    }

    if (context && context->objects) {
        context->objects->SpawnEffect(position, "parry", 0.6f);
    }
    AudioManager::Instance().PlaySfx("parry");
    if (context && context->ui) {
        context->ui->ShowMessage("PERFECT PARRY!", 1.0f);
    }
}

// ===========================================================================
// 炸弹
// ===========================================================================

void Player::UseBomb() {
    if (bombCount <= 0) return;
    bombCount--;

    // 清除所有敌方子弹
    if (context && context->objects) {
        auto bullets = context->objects->GetEnemyBullets();
        for (auto* b : bullets) {
            b->Destroy();
        }
    }

    if (context && context->objects) {
        context->objects->SpawnEffect(position, "explosion", 1.0f);
    }
    AudioManager::Instance().PlaySfx("plane_crash");
}

// ===========================================================================
// 伤害与火力
// ===========================================================================

void Player::TakeDamage(int amount) {
    if (IsInvincible() && amount > 0) return;

    health -= amount;
    health = math::Clamp(health, 0, maxHealth);

    if (amount > 0 && health > 0) {
        invincibleTimer = 1.0f;
        if (powerLevel > 1) {
            powerLevel--;
        }
    }

    if (health <= 0) {
        health = 0;
        Destroy();
    }
}

void Player::SetPowerLevel(int lv) {
    powerLevel = lv;
    if (powerLevel < 1) powerLevel = 1;
    if (powerLevel > 5) powerLevel = 5;
}
