#include "objects/Enemy.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "managers/ObjectManager.h"
#include "managers/ScoreManager.h"
#include "managers/AudioManager.h"

#include <SFML/Graphics/Texture.hpp>

// ===========================================================================
// Enemy：所有敌机基类。
// ===========================================================================

Enemy::Enemy() {
    radius = 20.0f;
}

void Enemy::OnInit() {
    // 子类设置自己的纹理
}

void Enemy::OnUpdate(float dt) {
    if (!active) return;

    // 子类实现移动逻辑
    UpdateBehavior(dt);

    // 射击计时
    fireTimer += dt;
    if (fireTimer >= fireInterval) {
        fireTimer = 0.0f;
        Fire();
    }

    // 边界检查：超出屏幕下方后销毁
    if (position.y > config::WINDOW_HEIGHT + 60.0f) {
        Destroy();
        return;
    }
    // 左右也做边界限制（NormalEnemy 等可能偏出屏幕）
    if (position.x < -60.0f || position.x > config::WINDOW_WIDTH + 60.0f) {
        Destroy();
        return;
    }

    sprite.setPosition(position);
    sprite.setRotation(sf::degrees(rotation));
}

void Enemy::OnRender(sf::RenderTarget& target) {
    if (!active) return;
    target.draw(sprite);
}

void Enemy::TakeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;

        // 被击杀音效
        AudioManager::Instance().PlaySfx("plane_crash");

        // 生成爆炸特效
        if (context && context->objects) {
            context->objects->SpawnEffect(position, "explosion", 0.8f);
        }

        // 掉落道具
        if (dropItem != ItemType::None && context && context->objects) {
            context->objects->SpawnItem(dropItem, position);
        }

        // 给分
        if (context && context->score) {
            context->score->AddScore(scoreValue);
        }

        Destroy();
    } else {
        // 受伤音效
        AudioManager::Instance().PlaySfx("plane_hit_by");
    }
}

