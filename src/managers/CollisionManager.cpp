#pragma once

#include <SFML/System/Vector2.hpp>

struct GameContext;

// ===========================================================================
// CollisionManager：集中处理本帧所有碰撞判定。
// 采用圆形判定，按“阵营”配对：
//   玩家子弹 vs 敌机
//   敌方子弹 vs 玩家
//   敌机      vs 玩家
//   道具      vs 玩家
// 命中后通过对象自身的方法造成伤害/拾取，碰撞结果由各对象的 Destroy/标志驱动。
// ===========================================================================

class CollisionManager {
public:
    CollisionManager();
    void SetContext(GameContext* ctx);

    void CheckCollisions(); // 每帧调用，解析所有碰撞

    // 静态工具：两圆是否相交
    static bool CircleHit(const sf::Vector2f& a, float ra,
                          const sf::Vector2f& b, float rb);

private:
    GameContext* context = nullptr;

    void ResolvePlayerBulletsVsEnemies();
    void ResolveEnemyBulletsVsPlayer();
    void ResolveEnemiesVsPlayer();
    void ResolveItemsVsPlayer();
};
