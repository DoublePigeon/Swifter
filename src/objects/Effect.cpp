#pragma once

#include <string>

#include "core/GameObject.h"

// ===========================================================================
// Effect：纯视觉特效（如格挡光环、爆炸、闪避残影、文字“PERFECT!”）。
// 不参与碰撞，按 duration 播放缩放/淡出动画后自动销毁。
// 通过 effectName 区分具体表现。
// ===========================================================================

class Effect : public GameObject {
public:
    Effect();
    ~Effect() override = default;

    ObjectType GetType() const override { return ObjectType::Effect; }

    void OnInit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;
    void OnDestroy() override {}

    void Setup(const std::string& name, float duration);

    const std::string& GetName() const { return effectName; }
    bool IsFinished() const { return elapsed >= duration; }

private:
    std::string effectName;
    float duration = 0.5f;
    float elapsed = 0.0f;
};
