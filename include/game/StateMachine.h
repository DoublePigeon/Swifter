#pragma once

#include <memory>
#include <stack>

#include <SFML/Graphics/RenderTarget.hpp>

struct GameContext;
class GameState;

// ===========================================================================
// StateMachine：状态栈式状态机。
// ChangeState 替换栈顶（菜单 -> 关卡选择 -> 游戏等）；
// PushState/PopState 用于叠加暂停层（不销毁下层 PlayState）。
// ===========================================================================

class StateMachine {
public:
    StateMachine();
    void SetContext(GameContext* ctx);

    void ChangeState(std::unique_ptr<GameState> state); // 替换栈顶
    void PushState(std::unique_ptr<GameState> state);    // 叠加（暂停）
    void PopState();                                     // 弹出叠加层

    void Update(float dt);
    void Render(sf::RenderTarget& target);

    GameState* Current() const;
    bool IsEmpty() const { return states.empty(); }

private:
    GameContext* context = nullptr;
    std::stack<std::unique_ptr<GameState>> states;
};
