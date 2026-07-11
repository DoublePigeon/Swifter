#include "managers/UIManager.h"
#include "core/GameContext.h"
#include "core/Config.h"
#include "managers/ScoreManager.h"
#include "managers/ObjectManager.h"
#include "managers/LevelManager.h"
#include "objects/Player.h"
#include "objects/Boss.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <sstream>

// ===========================================================================
// UIManager：负责 HUD 与提示信息的绘制。
// ===========================================================================

UIManager::UIManager() = default;

void UIManager::SetContext(GameContext* ctx) {
    context = ctx;
    // 尝试加载默认字体
    [[maybe_unused]] bool fontLoaded = font.openFromFile("assets/fonts/BestTen-CRT.otf");
}

void UIManager::Update(float dt) {
    if (messageTimer > 0.0f) {
        messageTimer -= dt;
        if (messageTimer < 0.0f) {
            messageTimer = 0.0f;
            currentMessage.clear();
        }
    }
}

void UIManager::Render(sf::RenderTarget& target) {
    DrawHUD(target);

    // 居中提示消息
    if (IsMessageActive()) {
        sf::Text msgText(font, currentMessage, 28);
        msgText.setFillColor(sf::Color::Yellow);
        msgText.setOutlineColor(sf::Color::Black);
        msgText.setOutlineThickness(2.0f);
        sf::FloatRect bounds = msgText.getLocalBounds();
        msgText.setPosition({
            config::WINDOW_WIDTH / 2.0f - bounds.size.x / 2.0f,
            config::WINDOW_HEIGHT / 2.0f - bounds.size.y / 2.0f - 40.0f
        });
        target.draw(msgText);
    }
}

void UIManager::DrawHUD(sf::RenderTarget& target) {
    if (!context) return;

    auto* score = context->score;
    auto* player = context->objects ? context->objects->GetPlayer() : nullptr;
    auto* level = context->level;

    // === 左上：分数 / 历史最高 ===
    if (score) {
        sf::Text scoreText(font, "", 18);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setOutlineThickness(1.5f);

        std::ostringstream oss;
        oss << "Score: " << score->GetScore();
        scoreText.setString(oss.str());
        scoreText.setPosition({10.0f, 10.0f});
        target.draw(scoreText);

        std::ostringstream oss2;
        oss2 << "Hi: " << score->GetHighScore();
        scoreText.setString(oss2.str());
        scoreText.setPosition({10.0f, 32.0f});
        target.draw(scoreText);
    }

    // === 左下：自机血条、火力等级 ===
    if (player) {
        float barX = 10.0f;
        float barY = static_cast<float>(config::WINDOW_HEIGHT) - 50.0f;

        // 血条背景
        sf::RectangleShape hpBg({200.0f, 16.0f});
        hpBg.setPosition({barX, barY});
        hpBg.setFillColor(sf::Color(60, 60, 60));
        target.draw(hpBg);

        // 血条前景
        float hpRatio = static_cast<float>(player->GetHealth()) / static_cast<float>(player->GetMaxHealth());
        sf::RectangleShape hpFg({200.0f * hpRatio, 16.0f});
        hpFg.setPosition({barX, barY});
        hpFg.setFillColor(hpRatio > 0.3f ? sf::Color::Green : sf::Color::Red);
        target.draw(hpFg);

        // 火力等级
        sf::Text powerText(font, "", 16);
        powerText.setFillColor(sf::Color::Cyan);
        powerText.setOutlineColor(sf::Color::Black);
        powerText.setOutlineThickness(1.5f);
        std::ostringstream oss;
        oss << "Power: Lv." << player->GetPowerLevel();
        powerText.setString(oss.str());
        powerText.setPosition({10.0f, barY + 20.0f});
        target.draw(powerText);
    }

    // === 右下：格挡/闪避冷却条 ===
    if (player) {
        float rightX = static_cast<float>(config::WINDOW_WIDTH) - 160.0f;
        float barY = static_cast<float>(config::WINDOW_HEIGHT) - 65.0f;

        // 格挡冷却
        sf::Text pLabel(font, "Parry", 14);
        pLabel.setFillColor(sf::Color::White);
        pLabel.setOutlineColor(sf::Color::Black);
        pLabel.setOutlineThickness(1.0f);
        pLabel.setPosition({rightX, barY});
        target.draw(pLabel);

        float parryCd = player->GetParryCooldown();
        float parryMax = 1.4f;
        float parryRatio = 1.0f - (parryCd / parryMax);
        sf::RectangleShape pBg({140.0f, 10.0f});
        pBg.setPosition({rightX, barY + 18.0f});
        pBg.setFillColor(sf::Color(60, 60, 60));
        target.draw(pBg);
        sf::RectangleShape pFg({140.0f * parryRatio, 10.0f});
        pFg.setPosition({rightX, barY + 18.0f});
        pFg.setFillColor(sf::Color::Blue);
        target.draw(pFg);

        // 闪避冷却
        sf::Text dLabel(font, "Dodge", 14);
        dLabel.setFillColor(sf::Color::White);
        dLabel.setOutlineColor(sf::Color::Black);
        dLabel.setOutlineThickness(1.0f);
        dLabel.setPosition({rightX, barY + 34.0f});
        target.draw(dLabel);

        float dodgeCd = player->GetDodgeCooldown();
        float dodgeMax = 0.9f;
        float dodgeRatio = 1.0f - (dodgeCd / dodgeMax);
        sf::RectangleShape dBg({140.0f, 10.0f});
        dBg.setPosition({rightX, barY + 52.0f});
        dBg.setFillColor(sf::Color(60, 60, 60));
        target.draw(dBg);
        sf::RectangleShape dFg({140.0f * dodgeRatio, 10.0f});
        dFg.setPosition({rightX, barY + 52.0f});
        dFg.setFillColor(sf::Color(200, 100, 255));
        target.draw(dFg);
    }

    // === 顶部：BOSS 血条（若存在） ===
    if (context->objects) {
        auto enemies = context->objects->GetEnemies();
        Boss* boss = nullptr;
        for (auto* enemy : enemies) {
            if (auto* b = dynamic_cast<Boss*>(enemy)) {
                if (b->IsAlive()) {
                    boss = b;
                    break;
                }
            }
        }

        if (boss) {
            float barWidth = 400.0f;
            float barX = config::WINDOW_WIDTH / 2.0f - barWidth / 2.0f;
            float barY = 10.0f;

            // 背景
            sf::RectangleShape bg({barWidth, 20.0f});
            bg.setPosition({barX, barY});
            bg.setFillColor(sf::Color(60, 60, 60));
            bg.setOutlineColor(sf::Color::White);
            bg.setOutlineThickness(1.0f);
            target.draw(bg);

            // Boss 血量比例
            float ratio = static_cast<float>(boss->GetHealth()) / static_cast<float>(boss->GetMaxHealth());
            sf::RectangleShape fg({barWidth * ratio, 20.0f});
            fg.setPosition({barX, barY});
            fg.setFillColor(sf::Color::Red);
            target.draw(fg);

            // Boss 名称
            sf::Text bossLabel(font, "BOSS", 16);
            bossLabel.setFillColor(sf::Color::White);
            bossLabel.setOutlineColor(sf::Color::Black);
            bossLabel.setOutlineThickness(1.0f);
            sf::FloatRect labelBounds = bossLabel.getLocalBounds();
            bossLabel.setPosition({
                config::WINDOW_WIDTH / 2.0f - labelBounds.size.x / 2.0f,
                barY + 22.0f
            });
            target.draw(bossLabel);
        }
    }
}

void UIManager::ShowMessage(const std::string& msg, float duration) {
    currentMessage = msg;
    messageTimer = duration;
}
