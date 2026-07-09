#include "core/Game.h"

// ===========================================================================
// Game：游戏顶层，拥有主循环与所有“共享”资源/管理器。
//   - 窗口 (RenderWindow)
//   - 状态机 (StateMachine)
//   - 分数管理 (ScoreManager) —— 跨局持久
//   - 资源/音频 —— 通过单例访问，写入 context
// 主循环：事件 -> 更新 -> 渲染，帧刷新。
// 退出时保存最高分。
// ===========================================================================


 Game::Game() {

 }
 Game::~Game() {

 }

 bool Game::Init(int width = 800, int height = 600, const std::string& title = "Swifter") {

 }
 void Game::Run() {

 }

 void Game::Quit() {

 }

 void Game::HandleEvents() {

 }   // 轮询 SFML 事件 -> Input / 窗口关闭
 void Game::Update(float dt) {

 } // 状态机更新 + 输入清理
 void Game::Render() {

 }         // 状态机渲染 + 显示

 void Game::BuildContext() {
    
 }   // 填充 context 的共享部分