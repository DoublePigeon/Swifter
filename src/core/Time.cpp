#include "core/Time.h"
#include <SFML/System/Time.hpp>

// ===========================================================================
// 时间管理：提供 deltaTime、总时间、全局时间缩放以及子弹时间因子。
// 子弹时间因子只作用于敌方子弹（完美闪避触发），不影响自机。
// ===========================================================================

Time& Time::Instance() {
    static Time instance;
    return instance;
}

Time::Time() = default;

void Time::Reset() {
    deltaTime = 0.0f;
    totalTime = 0.0f;
    timeScale = 1.0f;
    bulletTimeFactor = 1.0f;
    clock.restart();
}

void Time::Update() {
    deltaTime = clock.restart().asSeconds() * timeScale;
    totalTime += deltaTime;
}

float Time::DeltaTime() const {
    return deltaTime;
}

float Time::TotalTime() const {
    return totalTime;
}

float Time::TimeScale() const {
    return timeScale;
}

void Time::SetTimeScale(float scale) {
    timeScale = scale;
}

float Time::BulletTimeFactor() const {
    return bulletTimeFactor;
}

void Time::SetBulletTimeFactor(float f) {
    bulletTimeFactor = f;
}
