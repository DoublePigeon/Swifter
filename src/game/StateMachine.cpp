#include "game/StateMachine.h"
#include "game/GameState.h"
#include "core/GameContext.h"
#include <cassert>

StateMachine::StateMachine() = default;

void StateMachine::SetContext(GameContext* ctx) {
    context = ctx;
}

void StateMachine::ChangeState(std::unique_ptr<GameState> state) {
    if (!states.empty()) {
        states.top()->OnExit();
        states.pop();
    }
    states.push(std::move(state));
    states.top()->OnEnter(context);
}

void StateMachine::ClearAndSetState(std::unique_ptr<GameState> state) {
    while (!states.empty()) {
        states.top()->OnExit();
        states.pop();
    }
    states.push(std::move(state));
    states.top()->OnEnter(context);
}

void StateMachine::PushState(std::unique_ptr<GameState> state) {
    states.push(std::move(state));
    states.top()->OnEnter(context);
}

void StateMachine::PopState() {
    if (!states.empty()) {
        states.top()->OnExit();
        states.pop();
    }
}

void StateMachine::Update(float dt) {
    if (!states.empty()) {
        states.top()->OnUpdate(dt);
    }
}

void StateMachine::Render(sf::RenderTarget& target) {
    if (!states.empty()) {
        states.top()->OnRender(target);
    }
}

GameState* StateMachine::Current() const {
    return states.empty() ? nullptr : states.top().get();
}
