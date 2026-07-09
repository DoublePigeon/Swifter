#pragma once

#include <SFML/Graphics/Text.hpp>

#include "game/GameState.h"

// ===========================================================================
// GameOverState：失败 / 胜利结算界面。
// 通过 victory 区分标题与文案；显示本局分数与历史最高分；
// 任意键（或延时后）返回主菜单。
// ===========================================================================

class GameOverState : public GameState {
public:
    explicit GameOverState(bool victory);

    StateID GetID() const override { return victory ? StateID::Victory : StateID::GameOver; }

    void OnEnter(GameContext* ctx) override;
    void OnExit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;

private:
    bool victory;

    sf::Text titleText;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Text hintText;

    float timer = 0.0f;      // 短暂延时后才接受按键，防误触
};
