#pragma once

// ===========================================================================
// 随机数工具：基于 mt19937 的全局 RNG
// ===========================================================================

namespace rng {
    // 用固定种子初始化（0 表示用随机设备）
    void Seed(unsigned int seed = 0);

    // [min, max) 浮点
    float Range(float min, float max);

    // [min, max] 整型（闭区间）
    int Range(int min, int max);

    // [0, 1) 浮点
    float Value();

    // 概率事件，probability 取 [0,1]
    bool Chance(float probability);
} // namespace rng
