#include "managers/CollisionManager.h"
#include "managers/ObjectManager.h"
#include "core/GameContext.h"

#include "objects/Player.h"
#include "objects/Bullet.h"
#include "objects/Enemy.h"
#include "objects/Item.h"

#include <cmath>

// ===========================================================================
// CollisionManager：集中处理本帧所有碰撞判定。
// ===========================================================================

CollisionManager::CollisionManager() = default;

void CollisionManager::SetContext(GameContext* ctx) {
    context = ctx;
}

bool CollisionManager::CircleHit(const sf::Vector2f& a, float ra,
                                  const sf::Vector2f& b, float rb) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distSq = dx * dx + dy * dy;
    float radSum = ra + rb;
    return distSq <= radSum * radSum;
}

void CollisionManager::CheckCollisions() {
    if (!context || !context->objects) return;

    ResolvePlayerBulletsVsEnemies();
    ResolveEnemyBulletsVsPlayer();
    ResolveEnemiesVsPlayer();
    ResolveItemsVsPlayer();
}

void CollisionManager::ResolvePlayerBulletsVsEnemies() {
    auto bullets = context->objects->GetPlayerBullets();
    auto enemies = context->objects->GetEnemies();

    for (auto* bullet : bullets) {
        if (bullet->IsDead()) continue;

        for (auto* enemy : enemies) {
            if (enemy->IsDead()) continue;

            if (CircleHit(bullet->GetPosition(), bullet->GetRadius(),
                          enemy->GetPosition(), enemy->GetRadius())) {
                enemy->TakeDamage(bullet->GetDamage());
                bullet->Destroy();
                break;
            }
        }
    }
}

void CollisionManager::ResolveEnemyBulletsVsPlayer() {
    auto* player = context->objects->GetPlayer();
    if (!player || player->IsDead() || player->IsInvincible()) return;

    auto bullets = context->objects->GetEnemyBullets();

    for (auto* bullet : bullets) {
        if (bullet->IsDead()) continue;

        if (CircleHit(bullet->GetPosition(), bullet->GetRadius(),
                      player->GetPosition(), player->GetRadius())) {
            player->TakeDamage(bullet->GetDamage());
            bullet->Destroy();
        }
    }
}

void CollisionManager::ResolveEnemiesVsPlayer() {
    auto* player = context->objects->GetPlayer();
    if (!player || player->IsDead() || player->IsInvincible()) return;

    auto enemies = context->objects->GetEnemies();

    for (auto* enemy : enemies) {
        if (enemy->IsDead()) continue;

        if (CircleHit(enemy->GetPosition(), enemy->GetRadius(),
                      player->GetPosition(), player->GetRadius())) {
            player->TakeDamage(1);
            enemy->TakeDamage(1);
        }
    }
}

void CollisionManager::ResolveItemsVsPlayer() {
    auto* player = context->objects->GetPlayer();
    if (!player || player->IsDead()) return;

    auto items = context->objects->GetObjectsByType<Item>();

    for (auto* item : items) {
        if (item->IsDead()) continue;

        if (CircleHit(item->GetPosition(), item->GetRadius(),
                      player->GetPosition(), player->GetRadius())) {
            item->ApplyToPlayer(player);
            item->Destroy();
        }
    }
}
