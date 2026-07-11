#include "data/LevelLoader.h"

#include <fstream>
#include <sstream>
#include <algorithm>

// ===========================================================================
// LevelLoader：静态工具类，负责读取关卡文件。
// ===========================================================================

bool LevelLoader::Load(const std::string& filename, std::vector<EnemySpawnEvent>& outEvents) {
    outEvents.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // 去除首尾空白
        auto trim = [](std::string& s) {
            s.erase(0, s.find_first_not_of(" \t\r\n"));
            s.erase(s.find_last_not_of(" \t\r\n") + 1);
        };
        trim(line);

        // 忽略空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        EnemySpawnEvent event;

        // 解析：时间 敌机类型 X Y 道具 [弹幕模式]
        std::string typeStr, itemStr, patternStr;

        if (!(iss >> event.spawnTime >> typeStr >> event.position.x >> event.position.y >> itemStr)) {
            continue; // 格式错误，跳过该行
        }

        // 解析敌机类型
        if (typeStr == "Normal") {
            event.type = EnemyType::Normal;
        } else if (typeStr == "Shooter") {
            event.type = EnemyType::Shooter;
        } else if (typeStr == "Boss") {
            event.type = EnemyType::Boss;
        } else {
            continue; // 未知类型
        }

        // 解析道具类型
        if (itemStr == "None") {
            event.dropItem = ItemType::None;
        } else if (itemStr == "Heal") {
            event.dropItem = ItemType::Heal;
        } else if (itemStr == "PowerUp") {
            event.dropItem = ItemType::PowerUp;
        } else if (itemStr == "Bomb") {
            event.dropItem = ItemType::Bomb;
        } else if (itemStr == "ScoreBonus") {
            event.dropItem = ItemType::ScoreBonus;
        } else if (itemStr == "Shield") {
            event.dropItem = ItemType::Shield;
        } else {
            event.dropItem = ItemType::None;
        }

        // 可选的弹幕模式
        if (iss >> patternStr) {
            event.pattern = patternStr;
        }

        outEvents.push_back(event);
    }

    // 按 spawnTime 排序
    std::sort(outEvents.begin(), outEvents.end(),
        [](const EnemySpawnEvent& a, const EnemySpawnEvent& b) {
            return a.spawnTime < b.spawnTime;
        });

    return true;
}

bool LevelLoader::LoadLevelList(const std::string& indexFile, std::vector<LevelInfo>& outLevels) {
    outLevels.clear();

    std::ifstream file(indexFile);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    int index = 0;
    while (std::getline(file, line)) {
        // 去除首尾空白
        auto trim = [](std::string& s) {
            s.erase(0, s.find_first_not_of(" \t\r\n"));
            s.erase(s.find_last_not_of(" \t\r\n") + 1);
        };
        trim(line);

        // 忽略空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // 格式：文件名:显示名:是否解锁(1/0)
        std::istringstream iss(line);
        std::string filename, displayName, unlockedStr;

        if (std::getline(iss, filename, ':') &&
            std::getline(iss, displayName, ':') &&
            std::getline(iss, unlockedStr)) {

            LevelInfo info;
            info.filename = filename;
            info.displayName = displayName;
            info.index = index++;
            info.unlocked = (unlockedStr == "1");

            outLevels.push_back(info);
        }
    }

    return true;
}
