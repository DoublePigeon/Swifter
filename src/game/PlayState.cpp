#include "game/PlayState.h"
#include "game/PauseState.h"
#include "game/GameOverState.h"
#include "game/StateMachine.h"
#include "core/GameContext.h"
#include "core/Config.h"
#include "core/Time.h"
#include "core/Input.h"
#include "managers/ObjectManager.h"
#include "managers/CollisionManager.h"
#include "managers/LevelManager.h"
#include "managers/ScoreManager.h"
#include "managers/AudioManager.h"
#include "objects/Player.h"

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

// ===========================================================================
// PlayState：实际游玩状态。
// ===========================================================================

PlayState::PlayState(const std::string& levelFile)
    : levelFile(levelFile) {
}

PlayState::~PlayState() = default;

void PlayState::OnEnter(GameContext* ctx) {
    context = ctx;
    SetupLocalContext(ctx);

    player = objects.SpawnPlayer({
        config::WINDOW_WIDTH / 2.0f,
        config::WINDOW_HEIGHT - 80.0f
    });

    level.LoadLevel(levelFile);
    level.Start();

    // 播放关卡对应 BGM
    {
        // 从关卡文件名推断 BGM 编号（如 level1.txt -> bgm_level1.ogg）
        std::string bgmPath = levelFile;
        // 提取关卡编号
        size_t pos = bgmPath.find("level");
        if (pos != std::string::npos) {
            std::string numPart = bgmPath.substr(pos + 5); // "1.txt" 等
            size_t dotPos = numPart.find('.');
            std::string levelNum = (dotPos != std::string::npos) ? numPart.substr(0, dotPos) : numPart;
            AudioManager::Instance().PlayMusic("assets/music/bgm_level" + levelNum + ".ogg", true);
        }
    }

    Time::Instance().Reset();
    Time::Instance().SetTimeScale(1.0f);
    Time::Instance().SetBulletTimeFactor(1.0f);

    started = true;
}

void PlayState::OnExit() {
    AudioManager::Instance().StopMusic();
    ClearLocalContext(context);
    started = false;
}

void PlayState::OnUpdate(float dt) {
    if (!started) return;

    // Esc 暂停
    if (Input::Instance().IsKeyPressed(sf::Keyboard::Key::Escape)) {
        context->stateMachine->PushState(std::make_unique<PauseState>());
        return;
    }

    Time::Instance().Update();
    float delta = Time::Instance().DeltaTime();

    level.Update(delta);
    objects.UpdateAll(delta);
    collision.CheckCollisions();
    ui.Update(delta);
    AudioManager::Instance().Update();

    CheckEndConditions();
}

void PlayState::OnRender(sf::RenderTarget& target) {
    if (!started) return;
    target.clear(sf::Color::Black);
    objects.RenderAll(target);
    ui.Render(target);
}

void PlayState::SetupLocalContext(GameContext* ctx) {
    objects.SetContext(ctx);
    collision.SetContext(ctx);
    level.SetContext(ctx);
    ui.SetContext(ctx);

    ctx->objects = &objects;
    ctx->collision = &collision;
    ctx->level = &level;
    ctx->ui = &ui;
}

void PlayState::ClearLocalContext(GameContext* ctx) {
    ctx->objects = nullptr;
    ctx->collision = nullptr;
    ctx->level = nullptr;
    ctx->ui = nullptr;
    ctx->player = nullptr;
    objects.Clear();
}

void PlayState::CheckEndConditions() {
    if (!context) return;

    auto* currentPlayer = context->player;
    if (!currentPlayer || currentPlayer->IsDead()) {
        if (context->score) context->score->CommitHighScore();
        context->stateMachine->ChangeState(std::make_unique<GameOverState>(false));
        return;
    }

    if (level.IsLevelComplete()) {
        if (context->score) context->score->CommitHighScore();
        context->stateMachine->ChangeState(std::make_unique<GameOverState>(true));
        return;
    }
}
