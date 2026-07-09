#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>

struct GameContext;

// ===========================================================================
// UIManager：负责 HUD 与提示信息的绘制。
//   - 左上：分数 / 历史最高
//   - 左下：自机血条、火力等级
//   - 右下：格挡/闪避冷却条
//   - 顶部：BOSS 血条（若存在）
//   - 居中：临时提示消息（如“完美格挡！”）
// ===========================================================================

class UIManager {
public:
    UIManager();
    void SetContext(GameContext* ctx);

    void Update(float dt);
    void Render(sf::RenderTarget& target);     // 绘制全部 UI
    void DrawHUD(sf::RenderTarget& target);    // 仅绘制 HUD

    void ShowMessage(const std::string& msg, float duration); // 居中提示
    bool IsMessageActive() const { return messageTimer > 0.0f; }

private:
    GameContext* context = nullptr;
    sf::Font  font;
    std::string currentMessage;
    float messageTimer = 0.0f;
};
