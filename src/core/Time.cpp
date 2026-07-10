#include <core/Time.h>

// ===========================================================================
// 时间管理：提供 deltaTime、总时间、全局时间缩放以及子弹时间因子。
// 子弹时间因子只作用于敌方子弹（完美闪避触发），不影响自机。
// ===========================================================================

class Time {
public:
    static Time& Instance();

    void Reset();
    void Update(); // 每帧调用，刷新 deltaTime / totalTime

    float DeltaTime() const;     // 本帧增量（秒）
    float TotalTime() const;     // 自启动以来总时间（秒）

    float TimeScale() const;             // 全局时间缩放（暂停用 0）
    void  SetTimeScale(float scale);

    float BulletTimeFactor() const;      // 敌弹专用时间因子（1.0 正常）
    void  SetBulletTimeFactor(float f);  // 完美闪避后设为 < 1

private:
    Time();

    sf::Clock clock;
    float deltaTime = 0.0f;
    float totalTime = 0.0f;
    float timeScale = 1.0f;
    float bulletTimeFactor = 1.0f;
};
