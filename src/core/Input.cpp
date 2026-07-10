#include "core/Input.h"

// ===========================================================================
// 输入管理：区分“按住 / 本帧按下 / 本帧抬起”，方便实现格挡、闪避等触发式操作。
// 用法：Game::HandleEvents 中调用 HandleEvent，每帧末尾调用 Update 清理。
// ===========================================================================

Input& Input::Instance() {
    static Input instance;
    return instance;
}

void Input::HandleEvent(const sf::Event& event) {
    if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
        sf::Keyboard::Key key = keyPress->code;

        if (key != sf::Keyboard::Key::Unknown) {
            int keyCode = static_cast<int>(key);
            if (heldKeys.find(keyCode) == heldKeys.end()) {
                pressedKeys.insert(keyCode);
            }
            heldKeys.insert(keyCode);
        }
    }else if (const auto* keyRelease = event.getIf<sf::Event::KeyReleased>()) {
        sf::Keyboard::Key key = keyRelease->code;

        if (key != sf::Keyboard::Key::Unknown) {
            int keyCode = static_cast<int>(key);
            releasedKeys.insert(keyCode);
            heldKeys.erase(keyCode);
        }
    }
}

void Input::Update() {
    pressedKeys.clear();
    releasedKeys.clear();
} // 每帧末尾调用，清空 pressed/released

bool Input::IsKeyHeld(sf::Keyboard::Key key) const {
    return heldKeys.count(static_cast<int>(key)) > 0;
}   // 持续按住

bool Input::IsKeyPressed(sf::Keyboard::Key key) const {
    return pressedKeys.count(static_cast<int>(key)) > 0;
} // 本帧刚按下

bool Input::IsKeyReleased(sf::Keyboard::Key key) const {
    return releasedKeys.count(static_cast<int>(key)) > 0;
} // 本帧刚抬起