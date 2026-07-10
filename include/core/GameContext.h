#pragma once

// ===========================================================================
// GameContext：全局上下文，集中持有各管理器的裸指针。
// Game 创建并填充“共享”部分（窗口、资源、音频、分数）；
// PlayState 填充“局内”部分（对象、碰撞、关卡、UI、玩家），退出时置空。
// 所有对象通过 GameObject::context 访问各管理器，避免单例满天飞。
// ===========================================================================

class StateMachine;
class ObjectManager;
class ResourceManager;
class AudioManager;
class ScoreManager;
class UIManager;
class CollisionManager;
class LevelManager;
class Player;

namespace sf { class RenderWindow; }

struct GameContext {
    // —— 共享（由 Game 持有）——
    sf::RenderWindow* window       = nullptr;
    ResourceManager*  resources    = nullptr;
    AudioManager*     audio        = nullptr;
    ScoreManager*     score        = nullptr;
    StateMachine*     stateMachine = nullptr;

    // —— 局内（由 PlayState 持有）——
    ObjectManager*    objects   = nullptr;
    CollisionManager* collision = nullptr;
    LevelManager*     level     = nullptr;
    UIManager*        ui        = nullptr;
    Player*           player    = nullptr;
};
