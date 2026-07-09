#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include "core/Types.h"

struct GameContext;

// ===========================================================================
// GameState：游戏状态基类。
// 状态机通过 OnEnter/OnExit/OnUpdate/OnRender 驱动各状态。
// 状态本身不持有渲染窗口，通过 context 访问。
// ===========================================================================

class GameState {
public:
    virtual ~GameState() = default;

    virtual void OnEnter(GameContext* ctx) = 0;
    virtual void OnExit() = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnRender(sf::RenderTarget& target) = 0;

    virtual StateID GetID() const = 0;

protected:
    GameContext* context = nullptr;
};
