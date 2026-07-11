#include "core/Game.h"
#include "core/Input.h"
#include "core/Time.h"
#include "core/ResourceManager.h"
#include "managers/AudioManager.h"
#include "managers/ScoreManager.h"
#include "game/StateMachine.h"
#include "game/MenuState.h"

// ===========================================================================
// Game：核心骨架，拥有窗口、状态机、分数管理器，驱动主循环。
// ===========================================================================

Game::Game() = default;

// 析构须在 StateMachine / ScoreManager 完整类型可见后定义（unique_ptr 要求）
Game::~Game() = default;

bool Game::Init(int width, int height, const std::string& title) {
    // 1. 创建窗口
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode({ static_cast<unsigned>(width), static_cast<unsigned>(height) }),
        title);
    window->setFramerateLimit(60);

    // 2. 创建跨局持久的管理器
    scoreManager = std::make_unique<ScoreManager>();
    scoreManager->LoadHighScore();

    stateMachine = std::make_unique<StateMachine>();

    // 3. 填充共享上下文
    BuildContext();

    // 4. 给状态机设置上下文
    stateMachine->SetContext(&context);

    // 5. 注册所有音效别名 -> 文件路径
    auto& audio = AudioManager::Instance();
    audio.RegisterSfx("shoot_small", "assets/sounds/shoot_small.ogg");
    audio.RegisterSfx("shoot_big",   "assets/sounds/shoot_big.ogg");
    audio.RegisterSfx("evade",       "assets/sounds/evade.ogg");
    audio.RegisterSfx("parry",       "assets/sounds/parry.ogg");
    audio.RegisterSfx("plane_crash", "assets/sounds/plane_crash.ogg");
    audio.RegisterSfx("plane_hit_by","assets/sounds/plane_hit_by.ogg");
    audio.RegisterSfx("dummy",       "assets/sounds/dummy.ogg");

    // 6. 启动菜单
    stateMachine->ChangeState(std::make_unique<MenuState>());

    running = true;
    return true;
}

void Game::Run() {
    Time::Instance().Reset();

    while (running) {
        HandleEvents();
        Time::Instance().Update();
        float dt = Time::Instance().DeltaTime();
        Update(dt);
        Render();
    }
}

void Game::Quit() {
    running = false;
}

// ---- 私有方法 ----

void Game::HandleEvents() {
    while (auto event = window->pollEvent()) {
        Input::Instance().HandleEvent(*event);

        if (event->is<sf::Event::Closed>()) {
            Quit();
        }
    }
}

void Game::Update(float dt) {
    stateMachine->Update(dt);
    Input::Instance().Update(); // 清理单帧按键状态
}

void Game::Render() {
    window->clear(sf::Color(20, 20, 40)); // 深蓝黑背景
    stateMachine->Render(*window);
    window->display();
}

void Game::BuildContext() {
    context.window       = window.get();
    context.resources    = &ResourceManager::Instance();
    context.audio        = &AudioManager::Instance();
    context.score        = scoreManager.get();
    context.stateMachine = stateMachine.get();
}