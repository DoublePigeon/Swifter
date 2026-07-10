#include <core/GameObject.h>
#include <SFML/Graphics/Texture.hpp>

// ===========================================================================
// GameObject：所有屏幕实体的基类，提供类似 Unity MonoBehaviour 的生命周期。
// 大部分 getter/setter 已在头文件中 inline 定义，此处仅实现需要逻辑的方法。
// ===========================================================================

namespace {
    sf::Texture& DummyTexture() {
        static sf::Texture tex;
        tex.resize({1u, 1u});
        return tex;
    }
}

GameObject::GameObject()
    : context(nullptr)
    , position(0.f, 0.f)
    , rotation(0.f)
    , radius(16.f)
    , active(true)
    , dead(false)
    , sprite(DummyTexture()) {
}

void GameObject::SetContext(GameContext* ctx) {
    context = ctx;
}

void GameObject::Destroy() {
    dead   = true;
    active = false;
}