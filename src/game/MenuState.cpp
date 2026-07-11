#include "game/MenuState.h"
#include "game/PlayState.h"
#include "game/LevelSelectState.h"
#include "game/StateMachine.h"
#include "core/Input.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

// ===========================================================================
// MenuState：主菜单，标题 + 三个选项，上下选择回车确认。
// ===========================================================================

namespace {
    sf::Font& DummyFont() {
        static sf::Font font;
        return font;
    }
}

MenuState::MenuState()
    : titleText(DummyFont()) {
}

void MenuState::OnEnter(GameContext* ctx) {
    context = ctx;

    // 尝试通过 ResourceManager 获取字体（暂为桩实现，后续由别人填充）
    const sf::Font& font = context->resources->GetFont("assets/fonts/BestTen-CRT.otf");

    // 标题
    titleText.setFont(font);
    titleText.setString("Swifter");
    titleText.setCharacterSize(64);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    titleText.setPosition({ 400.f, 150.f });

    // 菜单项
    itemTexts.clear();
    for (size_t i = 0; i < items.size(); ++i) {
        itemTexts.emplace_back(font, items[i], 36);
        itemTexts[i].setFillColor(sf::Color(180, 180, 180));
        sf::FloatRect b = itemTexts[i].getLocalBounds();
        itemTexts[i].setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        itemTexts[i].setPosition({ 400.f, 300.f + i * 60.f });
    }

    // 高亮首项
    selected = 0;
    itemTexts[selected].setFillColor(sf::Color::Yellow);
}

void MenuState::OnExit() {
    // 无需特殊清理
}

void MenuState::OnUpdate(float dt) {
    auto& input = Input::Instance();

    if (input.IsKeyPressed(sf::Keyboard::Key::Up) ||
        input.IsKeyPressed(sf::Keyboard::Key::W)) {
        UpdateSelection(-1);
    }

    if (input.IsKeyPressed(sf::Keyboard::Key::Down) ||
        input.IsKeyPressed(sf::Keyboard::Key::S)) {
        UpdateSelection(1);
    }

    if (input.IsKeyPressed(sf::Keyboard::Key::Enter) ||
        input.IsKeyPressed(sf::Keyboard::Key::Space)) {
        switch (selected) {
        case 0: // 开始游戏
            context->stateMachine->ChangeState(
                std::make_unique<PlayState>("assets/levels/level1.txt"));
            break;
        case 1: // 选择关卡
            context->stateMachine->ChangeState(
                std::make_unique<LevelSelectState>());
            break;
        case 2: // 退出
            context->window->close();
            break;
        }
    }
}

void MenuState::OnRender(sf::RenderTarget& target) {
    target.draw(titleText);
    for (auto& t : itemTexts) {
        target.draw(t);
    }
}

void MenuState::UpdateSelection(int delta) {
    itemTexts[selected].setFillColor(sf::Color(180, 180, 180));
    selected = (selected + delta + static_cast<int>(items.size())) % static_cast<int>(items.size());
    itemTexts[selected].setFillColor(sf::Color::Yellow);
}
