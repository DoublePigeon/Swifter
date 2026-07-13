#include "objects/Effect.h"
#include "core/GameContext.h"
#include "core/ResourceManager.h"
#include "core/Config.h"

#include <SFML/Graphics/Texture.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

// ===========================================================================
// Effect：纯视觉特效（格挡光环、爆炸、闪避残影）。
// ===========================================================================

Effect::Effect() {
    radius = 0.0f; // 不参与碰撞
}

void Effect::OnInit() {
    // 纹理由 Setup 中的 effectName 决定
}

void Effect::Setup(const std::string& name, float dur) {
    effectName = name;
    duration = dur;
    elapsed = 0.0f;

    // 根据效果名设置纹理
    if (name == "parry") {
        sprite.setTexture(context->resources->GetTexture("assets/images/parry.png"), true);
    } else if (name == "evade") {
        sprite.setTexture(context->resources->GetTexture("assets/images/evade.png"), true);
    } else if (name == "explosion") {
        sprite.setTexture(context->resources->GetTexture("assets/images/explosion.png"), true);
    } else {
        // 未知效果：使用默认 dummy
        sprite.setTexture(context->resources->GetTexture("assets/images/dummy.png"), true);
    }

    auto texSize = sprite.getTexture().getSize();
    sprite.setOrigin({static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f});
    baseScale = config::SPRITE_EFFECT_W / static_cast<float>(texSize.x);
    sprite.setScale({baseScale, baseScale});
}

void Effect::OnUpdate(float dt) {
    if (!active) return;

    elapsed += dt;
    if (elapsed >= duration) {
        Destroy();
        return;
    }

    // 缩放动画：从小变大
    float progress = elapsed / duration;
    float animScale = baseScale * (1.0f + progress * 1.5f); // 逐渐放大
    sprite.setScale({animScale, animScale});

    // 透明度：逐渐淡出
    float alpha = 255.0f * (1.0f - progress);
    sf::Color col = sprite.getColor();
    col.a = static_cast<std::uint8_t>(alpha);
    sprite.setColor(col);

    sprite.setPosition(position);
}

void Effect::OnRender(sf::RenderTarget& target) {
    if (!active) return;
    target.draw(sprite);
}

