#pragma once

#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp>

#include "core/GameObject.h"
#include "core/Types.h"

// 前向声明，避免头文件互相包含
class Player;
class Bullet;
class Enemy;
class Item;
class Effect;
struct GameContext;

// ===========================================================================
// ObjectManager：统一管理所有屏幕实体。
// 使用 shared_ptr 持有对象，提供按类型查询、生成、回收等能力。
// 负责每帧调用所有对象的 OnUpdate/OnRender，并清理已标记 dead 的对象。
// ===========================================================================

class ObjectManager {
public:
    ObjectManager();
    ~ObjectManager();

    void SetContext(GameContext* ctx);

    // —— 生成便捷接口（返回裸指针便于直接操作）——
    Player* SpawnPlayer(const sf::Vector2f& pos);
    Enemy*  SpawnEnemy(EnemyType type, const sf::Vector2f& pos, ItemType drop = ItemType::None);
    Bullet* SpawnBullet(const sf::Vector2f& pos, const sf::Vector2f& velocity,
                        Faction faction, bool variant = false);
    Item*   SpawnItem(ItemType type, const sf::Vector2f& pos);
    Effect* SpawnEffect(const sf::Vector2f& pos, const std::string& effectName, float duration = 0.5f);

    // 通用添加（对象会被调用 OnInit）
    void AddObject(std::shared_ptr<GameObject> obj);

    // —— 生命周期 ——
    void UpdateAll(float dt);
    void RenderAll(sf::RenderTarget& target);
    void ClearDead();  // 移除 dead/出界 对象，调用 OnDestroy
    void Clear();      // 清空全部

    // —— 查询 ——
    const std::vector<std::shared_ptr<GameObject>>& GetObjects() const { return objects; }

    std::vector<Bullet*> GetEnemyBullets() const;   // 给格挡/闪避检测用
    std::vector<Bullet*> GetPlayerBullets() const;
    std::vector<Enemy*>  GetEnemies() const;
    Player*              GetPlayer() const;

    // 通用按类型查询（利用 dynamic_cast）
    template<typename T>
    std::vector<T*> GetObjectsByType() const {
        std::vector<T*> result;
        for (auto& obj : objects) {
            if (auto p = dynamic_cast<T*>(obj.get())) result.push_back(p);
        }
        return result;
    }

private:
    GameContext* context = nullptr;
    std::vector<std::shared_ptr<GameObject>> objects;
    Player* playerRef = nullptr; // 便捷弱引用（对象仍由 objects 持有）
};
