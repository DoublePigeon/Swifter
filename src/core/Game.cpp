#pragma once

#include <memory>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "core/GameContext.h"
#include "core/Types.h"

class StateMachine;
class ScoreManager;

// ===========================================================================
// Game：游戏顶层，拥有主循环与所有“共享”资源/管理器。
//   - 窗口 (RenderWindow)
//   - 状态机 (StateMachine)
//   - 分数管理 (ScoreManager) —— 跨局持久
//   - 资源/音频 —— 通过单例访问，写入 context
// 主循环：事件 -> 更新 -> 渲染，帧刷新。
// 退出时保存最高分。
// ===========================================================================

class Game {
public:
    Game();
    ~Game();

    bool Init(int width = 800, int height = 600,
              const std::string& title = "Swifter");
    void Run();
    void Quit();

private:
    void HandleEvents();   // 轮询 SFML 事件 -> Input / 窗口关闭
    void Update(float dt); // 状态机更新 + 输入清理
    void Render();         // 状态机渲染 + 显示

    void BuildContext();   // 填充 context 的共享部分

    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<StateMachine>     stateMachine;
    std::unique_ptr<ScoreManager>     scoreManager;

    GameContext context;
    bool running = false;
};
