#pragma once

#include <string>

#include "game/GameState.h"
#include "managers/ObjectManager.h"
#include "managers/CollisionManager.h"
#include "managers/LevelManager.h"
#include "managers/UIManager.h"

class Player;

// ===========================================================================
// PlayState：实际游玩状态。
// 持有本局所有“局内管理器”（对象/碰撞/关卡/UI），并把它们注册进共享
// GameContext，使对象们能通过 context 访问。退出时清空这些指针。
// 生命周期：
//   OnEnter  -> 注册局内管理器 -> 生成玩家 -> 加载关卡
//   OnUpdate -> 关卡驱动 / 输入 / 对象更新 / 碰撞 / 回收 / 终局判定
//   OnRender -> 背景 / 对象 / UI
//   OnExit   -> 提交分数 / 清理
// 终局：玩家死亡 -> GameOver；关卡完成 -> Victory。
// ===========================================================================

class PlayState : public GameState {
public:
    explicit PlayState(const std::string& levelFile);
    ~PlayState() override;

    StateID GetID() const override { return StateID::Play; }

    void OnEnter(GameContext* ctx) override;
    void OnExit() override;
    void OnUpdate(float dt) override;
    void OnRender(sf::RenderTarget& target) override;

private:
    void SetupLocalContext(GameContext* ctx); // 把局内管理器写进共享 context
    void ClearLocalContext(GameContext* ctx); // 退出时复位
    void CheckEndConditions();                // 胜负判定 -> 切状态

    std::string levelFile;

    // 局内管理器（由本状态持有）
    ObjectManager   objects;
    CollisionManager collision;
    LevelManager    level;
    UIManager       ui;

    Player* player = nullptr; // 由 objects 持有
    bool    started = false;
};
