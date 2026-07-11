#include "game/LevelSelectState.h"
#include "game/PlayState.h"
#include "game/MenuState.h"
#include "game/StateMachine.h"
#include "core/GameContext.h"
#include "core/Input.h"
#include "core/ResourceManager.h"
#include "core/Config.h"
#include "data/LevelLoader.h"

#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <sstream>

namespace {
    sf::Font& DummyFontForLS() {
        static sf::Font font;
        return font;
    }
}

LevelSelectState::LevelSelectState()
    : titleText(DummyFontForLS()) {
}

void LevelSelectState::OnEnter(GameContext* ctx) {
    context = ctx;
    selected = 0;

    LoadLevels();

    const sf::Font& font = ctx->resources->GetFont("assets/fonts/BestTen-CRT.otf");

    titleText.setFont(font);
    titleText.setString("Select Level");
    titleText.setCharacterSize(44);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect tb = titleText.getLocalBounds();
    titleText.setOrigin({tb.size.x / 2.f, tb.size.y / 2.f});
    titleText.setPosition({config::WINDOW_WIDTH / 2.0f, 120.0f});

    levelTexts.clear();
    for (size_t i = 0; i < levels.size(); ++i) {
        levelTexts.emplace_back(font, levels[i].displayName, 30);
        levelTexts[i].setFillColor(sf::Color(180, 180, 180));
        sf::FloatRect b = levelTexts[i].getLocalBounds();
        levelTexts[i].setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        levelTexts[i].setPosition({config::WINDOW_WIDTH / 2.0f, 250.0f + i * 50.0f});
    }
    if (!levelTexts.empty()) {
        levelTexts[selected].setFillColor(sf::Color::Yellow);
    }
}

void LevelSelectState::OnExit() {
}

void LevelSelectState::OnUpdate(float dt) {
    if (levels.empty()) return;

    auto& input = Input::Instance();

    if (input.IsKeyPressed(sf::Keyboard::Key::Up) || input.IsKeyPressed(sf::Keyboard::Key::W)) {
        UpdateSelection(-1);
    }
    if (input.IsKeyPressed(sf::Keyboard::Key::Down) || input.IsKeyPressed(sf::Keyboard::Key::S)) {
        UpdateSelection(1);
    }

    if (input.IsKeyPressed(sf::Keyboard::Key::Enter) || input.IsKeyPressed(sf::Keyboard::Key::Space)) {
        // 进入所选关卡
        context->stateMachine->ChangeState(
            std::make_unique<PlayState>(levels[selected].filename));
    }

    if (input.IsKeyPressed(sf::Keyboard::Key::Escape)) {
        context->stateMachine->ChangeState(std::make_unique<MenuState>());
    }
}

void LevelSelectState::OnRender(sf::RenderTarget& target) {
    target.draw(titleText);
    for (auto& t : levelTexts) {
        target.draw(t);
    }
}

void LevelSelectState::UpdateSelection(int delta) {
    if (levels.empty()) return;
    levelTexts[selected].setFillColor(sf::Color(180, 180, 180));
    selected = (selected + delta + static_cast<int>(levels.size())) % static_cast<int>(levels.size());
    levelTexts[selected].setFillColor(sf::Color::Yellow);
}

void LevelSelectState::LoadLevels() {
    levels.clear();

    // 使用 LevelLoader 统一解析冒号分隔的索引文件
    std::vector<LevelInfo> loaded;
    if (!LevelLoader::LoadLevelList(config::LEVEL_INDEX, loaded)) {
        return;
    }

    for (auto& info : loaded) {
        // 将相对文件名补全为完整路径
        info.filename = std::string(config::LEVELS_DIR) + info.filename;
        levels.push_back(info);
    }
}
