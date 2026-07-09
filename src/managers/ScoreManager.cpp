#pragma once

#include <string>

// ===========================================================================
// ScoreManager：管理本局分数与历史最高分，支持持久化。
// 重启游戏后通过 LoadHighScore 重新加载历史最高分。
// ===========================================================================

class ScoreManager {
public:
    ScoreManager();

    void LoadHighScore(const std::string& filename = "score.dat");
    void SaveHighScore(const std::string& filename = "score.dat");

    int  GetScore() const { return score; }
    int  GetHighScore() const { return highScore; }

    void AddScore(int amount);
    void ResetScore();

    // 本局结束时调用：若当前分 > 最高分则更新并保存
    void CommitHighScore();

private:
    int  score = 0;
    int  highScore = 0;
    std::string saveFile;
};
