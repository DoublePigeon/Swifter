#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics/Text.hpp>

#include "game/GameState.h"

// ===========================================================================
// MenuState：主菜单。
// 选项：开始游戏 / 选择关卡 / 退出；上下选择，回车确认。
// ===========================================================================

class MenuState : public GameState {
public:
    MenuState();

    StateID GetID() const override { return StateID::Menu; }

    void OnEnter(GameContext* ctx) override;
    void OnExit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;

private:
    void UpdateSelection(int delta);

    std::vector<std::string> items = { "Start Game", "Select Level", "Exit" };
    int selected = 0;

    sf::Text titleText;
    std::vector<sf::Text> itemTexts;
};
