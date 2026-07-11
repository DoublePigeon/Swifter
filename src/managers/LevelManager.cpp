#include "managers/LevelManager.h"
#include "data/LevelLoader.h"
#include "managers/ObjectManager.h"
#include "core/GameContext.h"
#include "objects/Boss.h"

// ===========================================================================
// LevelManager：按时间轴驱动敌机生成。
// ===========================================================================

LevelManager::LevelManager() = default;

void LevelManager::SetContext(GameContext* ctx) {
    context = ctx;
}

bool LevelManager::LoadLevel(const std::string& filename) {
    std::vector<EnemySpawnEvent> events;
    if (!LevelLoader::Load(filename, events)) {
        return false;
    }

    // 清空旧队列，重新填充
    while (!spawnQueue.empty()) {
        spawnQueue.pop();
    }

    for (auto& event : events) {
        spawnQueue.push(event);
    }

    levelComplete = false;
    return true;
}

void LevelManager::Start() {
    levelTimer = 0.0f;
    levelComplete = false;
}

void LevelManager::Update(float dt) {
    if (levelComplete) return;

    levelTimer += dt;

    // 按时间轴生成敌机
    while (!spawnQueue.empty() && spawnQueue.front().spawnTime <= levelTimer) {
        const auto& event = spawnQueue.front();

        if (context && context->objects) {
            context->objects->SpawnEnemy(event.type, event.position, event.dropItem);
        }

        spawnQueue.pop();
    }

    // 检查是否完成：队列空且场上无敌机
    if (spawnQueue.empty() && context && context->objects) {
        auto enemies = context->objects->GetEnemies();
        if (enemies.empty()) {
            levelComplete = true;
        }
    }
}

bool LevelManager::IsLevelComplete() const {
    return levelComplete;
}

bool LevelManager::IsBossDefeated() const {
    if (!context || !context->objects) return false;

    auto enemies = context->objects->GetEnemies();
    for (auto* enemy : enemies) {
        if (dynamic_cast<Boss*>(enemy) && enemy->IsAlive()) {
            return false;
        }
    }

    // 检查队列中是否还有 Boss 还未出场
    auto queueCopy = spawnQueue;
    while (!queueCopy.empty()) {
        if (queueCopy.front().type == EnemyType::Boss) {
            return false;
        }
        queueCopy.pop();
    }

    return true;
}
