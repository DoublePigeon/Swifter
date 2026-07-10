#include "managers/ScoreManager.h"
#include <fstream>

ScoreManager::ScoreManager() = default;

void ScoreManager::LoadHighScore(const std::string& filename) {
    saveFile = filename;
    std::ifstream in(filename);
    if (in.is_open()) {
        in >> highScore;
    }
}

void ScoreManager::SaveHighScore(const std::string& filename) {
    std::ofstream out(filename);
    if (out.is_open()) {
        out << highScore;
    }
}

void ScoreManager::AddScore(int amount) {
    score += amount;
}

void ScoreManager::ResetScore() {
    score = 0;
}

void ScoreManager::CommitHighScore() {
    if (score > highScore) {
        highScore = score;
        SaveHighScore(saveFile);
    }
}
