#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_set>

// ===========================================================================
// 输入管理：区分“按住 / 本帧按下 / 本帧抬起”，方便实现格挡、闪避等触发式操作。
// 用法：Game::HandleEvents 中调用 HandleEvent，每帧末尾调用 Update 清理。
// ===========================================================================

class Input {
public:
    static Input& Instance();

    void HandleEvent(const sf::Event& event);
    void Update(); // 每帧末尾调用，清空 pressed/released

    bool IsKeyHeld(sf::Keyboard::Key key) const;     // 持续按住
    bool IsKeyPressed(sf::Keyboard::Key key) const;  // 本帧刚按下
    bool IsKeyReleased(sf::Keyboard::Key key) const; // 本帧刚抬起

private:
    Input() = default;

    std::unordered_set<int> heldKeys;     // 当前按住的键
    std::unordered_set<int> pressedKeys;  // 本帧按下的键
    std::unordered_set<int> releasedKeys; // 本帧抬起的键
};
