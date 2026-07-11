#include "core/Random.h"

#include <random>
#include <ctime>

// ===========================================================================
// 随机数工具：基于 mt19937 的全局 RNG
// ===========================================================================

namespace rng {

    namespace {
        std::mt19937& GetEngine() {
            static std::mt19937 engine([]() {
                unsigned int s = static_cast<unsigned int>(std::time(nullptr));
                // 混合种子以提升质量
                std::random_device rd;
                s ^= rd();
                return s;
            }());
            return engine;
        }
    }

    void Seed(unsigned int seed) {
        if (seed == 0) {
            std::random_device rd;
            GetEngine().seed(rd());
        } else {
            GetEngine().seed(seed);
        }
    }

    float Range(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(GetEngine());
    }

    int Range(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(GetEngine());
    }

    float Value() {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(GetEngine());
    }

    bool Chance(float probability) {
        return Value() < probability;
    }

} // namespace rng
