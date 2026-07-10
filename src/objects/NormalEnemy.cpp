#pragma once

#include "objects/Enemy.h"

// ===========================================================================
// NormalEnemy：普通直线下落敌机，不射击，碰到自机造成伤害。
// ===========================================================================

class NormalEnemy : public Enemy {
public:
    NormalEnemy();
    ~NormalEnemy() override = default;

    EnemyType GetEnemyType() const override { return EnemyType::Normal; }

    void OnInit() override;
    void UpdateBehavior(float dt) override;
};
