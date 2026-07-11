#include "objects/Boss.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "core/Math.h"
#include "core/Time.h"
#include "core/Random.h"
#include "managers/ObjectManager.h"
#include "objects/Player.h"
#include "objects/Bullet.h"

#include <SFML/Graphics/Texture.hpp>

// ===========================================================================
// Boss：BOSS，多阶段、多弹幕模式。
// ===========================================================================

Boss::Boss() {
    health = 150;
    maxHealth = 150;
    scoreValue = 5000;
    dropItem = ItemType::PowerUp;
    radius = 40.0f;
    fireInterval = 0.5f;
    patternInterval = 3.0f;
}

void Boss::OnInit() {
    sprite.setTexture(context->resources->GetTexture("assets/images/boss.png"));
    auto texSize = sprite.getTexture().getSize();
    sprite.setOrigin({static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f});

    // 初始位置：屏幕上方外侧
    position = {config::WINDOW_WIDTH / 2.0f, -80.0f};
    targetPos = {config::WINDOW_WIDTH / 2.0f, 80.0f};
}

void Boss::OnUpdate(float dt) {
    if (!active) return;

    if (!entered) {
        UpdateEntry(dt);
    } else {
        UpdateMovement(dt);

        // 弹幕模式切换
        patternTimer += dt;
        if (patternTimer >= patternInterval) {
            patternTimer = 0.0f;
            currentPatternIndex = rng::Range(0, 4); // 0~4 五种模式
        }

        // 射击计时
        fireTimer += dt;
        if (fireTimer >= fireInterval) {
            fireTimer = 0.0f;
            Fire();
        }
    }

    sprite.setPosition(position);
    sprite.setRotation(sf::degrees(rotation));

    // 阶段更新
    UpdatePhase();
}

void Boss::OnRender(sf::RenderTarget& target) {
    if (!active) return;

    // 受伤闪烁效果
    if (static_cast<int>(fireTimer * 10.0f) % 2 == 0) {
        target.draw(sprite);
    }
}

void Boss::Fire() {
    if (!context || !context->objects) return;

    float bulletSpeed = config::ENEMY_BULLET_SPEED * 1.2f;
    int count = 8 + (phase - 1) * 4; // 阶段越高子弹越多

    switch (currentPatternIndex) {
        case 0: FireCirclePattern(count, bulletSpeed); break;
        case 1: FireSpiralPattern(count, bulletSpeed, 15.0f + phase * 5.0f); break;
        case 2: FireSpreadPattern(count, bulletSpeed, 60.0f + phase * 10.0f); break;
        case 3: FireAimedPattern(bulletSpeed * 1.3f); break;
        case 4: FireWavePattern(bulletSpeed); break;
    }
}

void Boss::FireCirclePattern(int count, float speed) {
    for (int i = 0; i < count; ++i) {
        float angle = 360.0f / static_cast<float>(count) * static_cast<float>(i);
        sf::Vector2f dir = math::DirectionFromAngle(angle);
        context->objects->SpawnBullet(position, dir * speed, Faction::Enemy);
    }
}

void Boss::FireSpiralPattern(int count, float speed, float angleStep) {
    for (int i = 0; i < count; ++i) {
        float angle = spiralAngle + angleStep * static_cast<float>(i);
        sf::Vector2f dir = math::DirectionFromAngle(angle);
        context->objects->SpawnBullet(position, dir * speed, Faction::Enemy);
    }
    spiralAngle += 20.0f; // 旋转累加
}

void Boss::FireSpreadPattern(int count, float speed, float spreadAngle) {
    if (!context || !context->player) return;

    sf::Vector2f toPlayer = context->player->GetPosition() - position;
    float baseAngle = math::ToDegrees(std::atan2(toPlayer.y, toPlayer.x));
    float halfSpread = spreadAngle / 2.0f;

    for (int i = 0; i < count; ++i) {
        float t = (count == 1) ? 0.0f : static_cast<float>(i) / static_cast<float>(count - 1);
        float angle = baseAngle - halfSpread + t * spreadAngle;
        sf::Vector2f dir = math::DirectionFromAngle(angle);
        context->objects->SpawnBullet(position, dir * speed, Faction::Enemy);
    }
}

void Boss::FireAimedPattern(float speed) {
    if (!context || !context->player) return;

    sf::Vector2f toPlayer = context->player->GetPosition() - position;
    sf::Vector2f dir = math::Normalize(toPlayer);

    // 多发瞄准弹（带微小偏移）
    for (int i = -1; i <= 1; ++i) {
        float offsetAngle = static_cast<float>(i) * 8.0f;
        float base = math::ToDegrees(std::atan2(dir.y, dir.x));
        sf::Vector2f adjusted = math::DirectionFromAngle(base + offsetAngle);
        context->objects->SpawnBullet(position, adjusted * speed, Faction::Enemy);
    }
}

void Boss::FireWavePattern(float speed) {
    if (!context || !context->player) return;

    float waveAmplitude = 40.0f;
    float waveFrequency = 0.03f;
    float totalTime = Time::Instance().TotalTime();

    // 波浪形弹幕：生成两列正弦波子弹
    for (int i = -5; i <= 5; ++i) {
        float xOffset = static_cast<float>(i) * 20.0f;
        float angle = 90.0f + std::sin(totalTime * 3.0f + static_cast<float>(i) * 0.5f) * waveAmplitude;
        sf::Vector2f dir = math::DirectionFromAngle(angle);
        sf::Vector2f spawnPos = position + sf::Vector2f(xOffset, 0.0f);
        context->objects->SpawnBullet(spawnPos, dir * speed, Faction::Enemy);
    }
}

void Boss::UpdateEntry(float dt) {
    // 入场动画：从屏幕上方滑入目标位置
    entryProgress += dt * 0.6f;
    if (entryProgress >= 1.0f) {
        entryProgress = 1.0f;
        position = targetPos;
        entered = true;
    } else {
        position.y = math::Lerp(-80.0f, targetPos.y, entryProgress);
    }
}

void Boss::UpdateMovement(float dt) {
    // 左右巡游
    float speed = 80.0f + static_cast<float>(phase) * 20.0f; // 阶段越高越快
    position.x += speed * moveDir * dt;

    // 边界反弹：Boss 在屏幕内左右移动，留 60px 边距
    float centerX = config::WINDOW_WIDTH / 2.0f;
    float amplitude = centerX - 60.0f; // 340px 振幅
    if (position.x > centerX + amplitude) {
        position.x = centerX + amplitude;
        moveDir = -1.0f;
    } else if (position.x < centerX - amplitude) {
        position.x = centerX - amplitude;
        moveDir = 1.0f;
    }
}

void Boss::UpdatePhase() {
    int totalPhases = maxPhases;
    float hpPerPhase = static_cast<float>(maxHealth) / static_cast<float>(totalPhases);

    int newPhase = totalPhases - static_cast<int>(static_cast<float>(health - 1) / hpPerPhase);
    if (newPhase < 1) newPhase = 1;
    if (newPhase > totalPhases) newPhase = totalPhases;

    if (newPhase != phase) {
        phase = newPhase;
        // 阶段越高，射击间隔越短
        fireInterval = 0.5f - static_cast<float>(phase - 1) * 0.15f;
        if (fireInterval < 0.15f) fireInterval = 0.15f;
        patternInterval = 3.0f - static_cast<float>(phase - 1) * 0.5f;
        if (patternInterval < 1.0f) patternInterval = 1.0f;
    }
}

float Boss::GetPhaseHealthRatio() const {
    if (phase <= 1) return 1.0f;
    float hpPerPhase = static_cast<float>(maxHealth) / static_cast<float>(maxPhases);
    float hpInPhase = static_cast<float>(health) - hpPerPhase * static_cast<float>(phase - 1);
    return hpInPhase / hpPerPhase;
}

