#pragma once

#include <SFML/System/Vector2.hpp>

#include "objects/Enemy.h"
#include "core/Types.h"

// ===========================================================================
// Boss：BOSS，特殊样式（更大、多阶段、专属血条、多弹幕模式）。
// 进入战场后做入场动画，随后在顶部左右巡游并周期性切换弹幕模式。
// 血量降到阈值进入下一阶段，弹幕更密集。
// 被击杀后关卡胜利。
// ===========================================================================

class Boss : public Enemy {
public:
    Boss();
    ~Boss() override = default;

    EnemyType GetEnemyType() const override { return EnemyType::Boss; }

    void OnInit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;
    void OnDestroy() override {}

    // —— 阶段 ——
    int   GetPhase() const { return phase; }
    int   GetMaxPhases() const { return maxPhases; }
    float GetPhaseHealthRatio() const; // 当前阶段血量比例（用于分阶段血条）

    // —— 弹幕模式 ——
    void Fire() override; // 按当前模式派发
    void FireCirclePattern(int count, float speed);
    void FireSpiralPattern(int count, float speed, float angleStep);
    void FireSpreadPattern(int count, float speed, float spreadAngle);
    void FireAimedPattern(float speed);
    void FireWavePattern(float speed);

private:
    void UpdateEntry(float dt);     // 入场动画
    void UpdateMovement(float dt);  // 巡游
    void UpdatePhase();             // 根据血量切阶段

    int   phase = 1;
    int   maxPhases = 3;
    float spiralAngle = 0.0f;       // 螺旋累积角
    float patternTimer = 0.0f;
    float patternInterval = 1.2f;
    int   currentPatternIndex = 0;

    bool  entered = false;          // 是否完成入场
    float entryProgress = 0.0f;
    sf::Vector2f targetPos;         // 巡游目标点
    float moveDir = 1.0f;           // 巡游方向
};
