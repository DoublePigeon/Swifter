#include "game/PauseState.h"
#include "game/MenuState.h"
#include "game/StateMachine.h"
#include "core/GameContext.h"
#include "core/Input.h"
#include "core/ResourceManager.h"
#include "core/Config.h"

#include <SFML/Window/Keyboard.hpp>

namespace {
    sf::Font& DummyFontForPause() {
        static sf::Font font;
        return font;
    }
}

PauseState::PauseState()
    : titleText(DummyFontForPause()) {
}

void PauseState::OnEnter(GameContext* ctx) {
    context = ctx;
    selected = 0;

    const sf::Font& font = ctx->resources->GetFont("assets/fonts/BestTen-CRT.otf");

    titleText.setFont(font);
    titleText.setString("PAUSED");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
    titleText.setPosition({config::WINDOW_WIDTH / 2.0f, 200.0f});

    itemTexts.clear();
    for (size_t i = 0; i < items.size(); ++i) {
        itemTexts.emplace_back(font, items[i], 32);
        itemTexts[i].setFillColor(sf::Color(180, 180, 180));
        sf::FloatRect b = itemTexts[i].getLocalBounds();
        itemTexts[i].setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        itemTexts[i].setPosition({config::WINDOW_WIDTH / 2.0f, 320.0f + i * 60.0f});
    }
    itemTexts[selected].setFillColor(sf::Color::Yellow);
}

void PauseState::OnExit() {
}

void PauseState::OnUpdate(float dt) {
    auto& input = Input::Instance();

    if (input.IsKeyPressed(sf::Keyboard::Key::Up) || input.IsKeyPressed(sf::Keyboard::Key::W)) {
        UpdateSelection(-1);
    }
    if (input.IsKeyPressed(sf::Keyboard::Key::Down) || input.IsKeyPressed(sf::Keyboard::Key::S)) {
        UpdateSelection(1);
    }

    if (input.IsKeyPressed(sf::Keyboard::Key::Enter) || input.IsKeyPressed(sf::Keyboard::Key::Space)) {
        if (selected == 0) {
            // 继续游戏 → 弹出暂停层
            context->stateMachine->PopState();
        } else {
            // 返回主菜单
            context->stateMachine->ChangeState(std::make_unique<MenuState>());
        }
    }

    // Esc 也继续
    if (input.IsKeyPressed(sf::Keyboard::Key::Escape)) {
        context->stateMachine->PopState();
    }
}

void PauseState::OnRender(sf::RenderTarget& target) {
    target.draw(titleText);
    for (auto& t : itemTexts) {
        target.draw(t);
    }
}

void PauseState::UpdateSelection(int delta) {
    itemTexts[selected].setFillColor(sf::Color(180, 180, 180));
    selected = (selected + delta + static_cast<int>(items.size())) % static_cast<int>(items.size());
    itemTexts[selected].setFillColor(sf::Color::Yellow);
}
