#pragma once

#include "objects/Enemy.h"

// ===========================================================================
// ShooterEnemy：射手型敌机，下落到某高度后悬停并周期性向自机发射子弹。
// 可掉落道具（常为 Heal / PowerUp）。
// ===========================================================================

class ShooterEnemy : public Enemy {
public:
    ShooterEnemy();
    ~ShooterEnemy() override = default;

    EnemyType GetEnemyType() const override { return EnemyType::Shooter; }

    void OnInit() override;
    void UpdateBehavior(float dt) override;
    void Fire() override; // 朝自机方向单发或散射

private:
    bool  hovering = false;     // 是否已到达悬停高度
    float hoverY = 120.0f;      // 悬停 Y 坐标
};
