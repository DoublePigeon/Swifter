#include "game/GameOverState.h"
#include "game/MenuState.h"
#include "game/StateMachine.h"
#include "core/GameContext.h"
#include "core/Input.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "managers/ScoreManager.h"

#include <SFML/Window/Keyboard.hpp>
#include <sstream>

namespace {
    sf::Font& DummyFontForGO() {
        static sf::Font font;
        return font;
    }
}

GameOverState::GameOverState(bool victory)
    : victory(victory)
    , titleText(DummyFontForGO())
    , scoreText(DummyFontForGO())
    , highScoreText(DummyFontForGO())
    , hintText(DummyFontForGO()) {
}

void GameOverState::OnEnter(GameContext* ctx) {
    context = ctx;
    timer = 0.0f;

    const sf::Font& font = context->resources->GetFont("assets/fonts/BestTen-CRT.otf");

    // 标题
    titleText.setFont(font);
    titleText.setString(victory ? "VICTORY!" : "GAME OVER");
    titleText.setCharacterSize(52);
    titleText.setFillColor(victory ? sf::Color::Yellow : sf::Color::Red);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
    titleText.setPosition({config::WINDOW_WIDTH / 2.0f, 150.0f});

    // 分数
    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::White);
    std::ostringstream oss;
    oss << "Score: " << (context->score ? context->score->GetScore() : 0);
    scoreText.setString(oss.str());
    sf::FloatRect sb = scoreText.getLocalBounds();
    scoreText.setOrigin({sb.size.x / 2.f, sb.size.y / 2.f});
    scoreText.setPosition({config::WINDOW_WIDTH / 2.0f, 250.0f});

    // 最高分
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color(200, 200, 200));
    std::ostringstream oss2;
    oss2 << "Best: " << (context->score ? context->score->GetHighScore() : 0);
    highScoreText.setString(oss2.str());
    sf::FloatRect hb = highScoreText.getLocalBounds();
    highScoreText.setOrigin({hb.size.x / 2.f, hb.size.y / 2.f});
    highScoreText.setPosition({config::WINDOW_WIDTH / 2.0f, 300.0f});

    // 提示
    hintText.setFont(font);
    hintText.setString("Press any key to return");
    hintText.setCharacterSize(20);
    hintText.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hintb = hintText.getLocalBounds();
    hintText.setOrigin({hintb.size.x / 2.f, hintb.size.y / 2.f});
    hintText.setPosition({config::WINDOW_WIDTH / 2.0f, 380.0f});
}

void GameOverState::OnExit() {
}

void GameOverState::OnUpdate(float dt) {
    timer += dt;

    // 延时 1.5 秒后方可跳过
    if (timer > 1.5f) {
        auto& input = Input::Instance();
        // 检查常用按键
        static const sf::Keyboard::Key checkKeys[] = {
            sf::Keyboard::Key::Enter, sf::Keyboard::Key::Space,
            sf::Keyboard::Key::Escape, sf::Keyboard::Key::Z,
            sf::Keyboard::Key::X, sf::Keyboard::Key::C,
            sf::Keyboard::Key::Up, sf::Keyboard::Key::Down,
            sf::Keyboard::Key::Left, sf::Keyboard::Key::Right
        };
        for (auto k : checkKeys) {
            if (input.IsKeyPressed(k)) {
                context->stateMachine->ChangeState(std::make_unique<MenuState>());
                return;
            }
        }
    }
}

void GameOverState::OnRender(sf::RenderTarget& target) {
    target.draw(titleText);
    target.draw(scoreText);
    target.draw(highScoreText);
    target.draw(hintText);
}
