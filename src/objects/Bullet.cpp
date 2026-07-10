#pragma once

#include <SFML/System/Vector2.hpp>

#include "core/GameObject.h"
#include "core/Types.h"

// ===========================================================================
// Bullet：通用子弹（玩家与敌方共用）。
//   - faction 区分阵营，决定与谁碰撞
//   - variant 标记玩家变体攻击子弹（更宽/更强）
//   - homing 标记追踪弹（BOSS 可用）
//   - Deflect() 由完美格挡调用，把敌方子弹弹反为玩家子弹
// 敌方子弹在 OnUpdate 中受 Time::BulletTimeFactor 影响（完美闪避子弹时间）。
// 出界或超过 maxLifeTime 自动销毁。
// ===========================================================================

class Bullet : public GameObject {
public:
    Bullet();
    ~Bullet() override = default;

    ObjectType GetType() const override { return ObjectType::Bullet; }

    void OnInit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;
    void OnDestroy() override {}

    // —— 速度 ——
    void  SetVelocity(const sf::Vector2f& v) { velocity = v; }
    const sf::Vector2f& GetVelocity() const { return velocity; }
    void  SetSpeed(float s) { speed = s; }
    float GetSpeed() const { return speed; }

    // —— 阵营与伤害 ——
    void    SetFaction(Faction f) { faction = f; }
    Faction GetFaction() const { return faction; }
    void    SetDamage(int d) { damage = d; }
    int     GetDamage() const { return damage; }

    // —— 变体攻击 ——
    void SetVariant(bool v) { variant = v; }
    bool IsVariant() const { return variant; }

    // —— 追踪 ——
    void SetHoming(bool h, float rate = 2.0f);
    bool IsHoming() const { return homing; }

    // —— 弹反（完美格挡）——
    void Deflect();
    bool IsDeflected() const { return deflected; }

    // —— 生命周期 ——
    void SetMaxLifeTime(float t) { maxLifeTime = t; }

private:
    sf::Vector2f velocity;
    float speed = 300.0f;
    int   damage = 1;
    Faction faction = Faction::Enemy;

    bool  variant = false;
    bool  homing = false;
    float homingRate = 2.0f;
    bool  deflected = false;

    float lifeTime = 0.0f;
    float maxLifeTime = 8.0f;
};
