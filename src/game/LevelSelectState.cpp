#pragma once

#include <vector>

#include <SFML/Graphics/Text.hpp>

#include "game/GameState.h"
#include "data/LevelData.h"

// ===========================================================================
// LevelSelectState：关卡选择。
// 从 index.txt 读取关卡列表，仅显示已解锁关卡；左右/上下切换，回车进入。
// ===========================================================================

class LevelSelectState : public GameState {
public:
    StateID GetID() const override { return StateID::LevelSelect; }

    void OnEnter(GameContext* ctx) override;
    void OnExit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;

private:
    void UpdateSelection(int delta);
    void LoadLevels();

    std::vector<LevelInfo> levels;
    int selected = 0;

    sf::Text titleText;
    std::vector<sf::Text> levelTexts;
};
