#pragma once

#include <vector>
#include <string>

#include "data/LevelData.h"

// ===========================================================================
// LevelLoader：静态工具类，负责读取关卡文件。
//
// 关卡文件格式（以 # 开头为注释，空行忽略）：
//   时间  敌机类型  X  Y  道具  [弹幕模式]
// 例:
//   2.0   Normal    200  -50  None
//   5.0   Shooter   300  -50  Heal
//   15.0  Boss      400  -100 None  Circle
//
// 关卡索引文件（index.txt）格式：
//   文件名:显示名:是否解锁(1/0)
// 例:
//   level1.txt:第一关 新手:1
// ===========================================================================

class LevelLoader {
public:
    // 解析单个关卡文件，结果按时间排序填入 outEvents
    static bool Load(const std::string& filename, std::vector<EnemySpawnEvent>& outEvents);

    // 读取关卡索引，得到全部关卡信息
    static bool LoadLevelList(const std::string& indexFile, std::vector<LevelInfo>& outLevels);
};
