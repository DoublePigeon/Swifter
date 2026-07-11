#include "managers/ObjectManager.h"
#include "core/GameContext.h"
#include "managers/ScoreManager.h"

#include "objects/Player.h"
#include "objects/Bullet.h"
#include "objects/Enemy.h"
#include "objects/NormalEnemy.h"
#include "objects/ShooterEnemy.h"
#include "objects/Boss.h"
#include "objects/Item.h"
#include "objects/Effect.h"

#include <algorithm>

// ===========================================================================
// ObjectManager：统一管理所有屏幕实体。
// ===========================================================================

ObjectManager::ObjectManager() = default;
ObjectManager::~ObjectManager() {
    Clear();
}

void ObjectManager::SetContext(GameContext* ctx) {
    context = ctx;
}

void ObjectManager::AddObject(std::shared_ptr<GameObject> obj) {
    if (!obj) return;
    obj->SetContext(context);
    obj->OnInit();
    objects.push_back(obj);
}

// —— 生成便捷接口 ——

Player* ObjectManager::SpawnPlayer(const sf::Vector2f& pos) {
    auto player = std::make_shared<Player>();
    player->SetPosition(pos);
    auto* raw = player.get();
    AddObject(player);
    playerRef = raw;
    // 同步到 GameContext
    if (context) {
        context->player = raw;
    }
    return raw;
}

Enemy* ObjectManager::SpawnEnemy(EnemyType type, const sf::Vector2f& pos, ItemType drop) {
    std::shared_ptr<Enemy> enemy;

    switch (type) {
        case EnemyType::Normal:
            enemy = std::make_shared<NormalEnemy>();
            break;
        case EnemyType::Shooter:
            enemy = std::make_shared<ShooterEnemy>();
            break;
        case EnemyType::Boss:
            enemy = std::make_shared<Boss>();
            break;
        default:
            enemy = std::make_shared<NormalEnemy>();
            break;
    }

    enemy->SetPosition(pos);
    enemy->SetDropItem(drop);
    auto* raw = enemy.get();
    AddObject(enemy);
    return raw;
}

Bullet* ObjectManager::SpawnBullet(const sf::Vector2f& pos, const sf::Vector2f& velocity,
                                    Faction faction, bool variant) {
    auto bullet = std::make_shared<Bullet>();
    bullet->SetPosition(pos);
    bullet->SetFaction(faction);
    bullet->SetVelocity(velocity);
    if (variant) {
        bullet->SetVariant(true);
    }
    auto* raw = bullet.get();
    AddObject(bullet);
    return raw;
}

Item* ObjectManager::SpawnItem(ItemType type, const sf::Vector2f& pos) {
    auto item = std::make_shared<Item>();
    item->SetItemType(type);
    item->SetPosition(pos);
    auto* raw = item.get();
    AddObject(item);
    return raw;
}

Effect* ObjectManager::SpawnEffect(const sf::Vector2f& pos, const std::string& effectName,
                                    float duration) {
    auto effect = std::make_shared<Effect>();
    effect->SetPosition(pos);
    effect->Setup(effectName, duration);
    auto* raw = effect.get();
    AddObject(effect);
    return raw;
}

// —— 生命周期 ——

void ObjectManager::UpdateAll(float dt) {
    for (auto& obj : objects) {
        if (obj->IsActive()) {
            obj->OnUpdate(dt);
        }
    }
    ClearDead();
}

void ObjectManager::RenderAll(sf::RenderTarget& target) {
    for (auto& obj : objects) {
        if (obj->IsActive()) {
            obj->OnRender(target);
        }
    }
}

void ObjectManager::ClearDead() {
    auto it = std::remove_if(objects.begin(), objects.end(),
        [](const std::shared_ptr<GameObject>& obj) {
            if (obj->IsDead()) {
                obj->OnDestroy();
                return true;
            }
            return false;
        });
    objects.erase(it, objects.end());

    // 检查 playerRef 是否已被移除
    if (playerRef && playerRef->IsDead()) {
        playerRef = nullptr;
        if (context) {
            context->player = nullptr;
        }
    }
}

void ObjectManager::Clear() {
    for (auto& obj : objects) {
        obj->OnDestroy();
    }
    objects.clear();
    playerRef = nullptr;
    if (context) {
        context->player = nullptr;
    }
}

// —— 查询 ——

std::vector<Bullet*> ObjectManager::GetEnemyBullets() const {
    std::vector<Bullet*> result;
    for (auto& obj : objects) {
        if (obj->IsDead()) continue;
        if (auto* bullet = dynamic_cast<Bullet*>(obj.get())) {
            if (bullet->GetFaction() == Faction::Enemy) {
                result.push_back(bullet);
            }
        }
    }
    return result;
}

std::vector<Bullet*> ObjectManager::GetPlayerBullets() const {
    std::vector<Bullet*> result;
    for (auto& obj : objects) {
        if (obj->IsDead()) continue;
        if (auto* bullet = dynamic_cast<Bullet*>(obj.get())) {
            if (bullet->GetFaction() == Faction::Player) {
                result.push_back(bullet);
            }
        }
    }
    return result;
}

std::vector<Enemy*> ObjectManager::GetEnemies() const {
    std::vector<Enemy*> result;
    for (auto& obj : objects) {
        if (obj->IsDead()) continue;
        if (auto* enemy = dynamic_cast<Enemy*>(obj.get())) {
            result.push_back(enemy);
        }
    }
    return result;
}

Player* ObjectManager::GetPlayer() const {
    return playerRef;
}
