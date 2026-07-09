#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics/Text.hpp>

#include "game/GameState.h"

// ===========================================================================
// PauseState：暂停叠加层（Push 到栈顶，不销毁下层 PlayState）。
// 选项：继续游戏 / 返回主菜单。Esc 也可继续。
// ===========================================================================

class PauseState : public GameState {
public:
    StateID GetID() const override { return StateID::Pause; }

    void OnEnter(GameContext* ctx) override;
    void OnExit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;

private:
    void UpdateSelection(int delta);

    std::vector<std::string> items = { "继续游戏", "返回主菜单" };
    int selected = 0;

    sf::Text titleText;
    std::vector<sf::Text> itemTexts;
};
