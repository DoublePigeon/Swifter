#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

// ===========================================================================
// 数学工具：向量长度、距离、归一化、角度转换、插值等
// ===========================================================================

namespace math {
    constexpr float PI = 3.14159265358979323846f;

    inline float Length(const sf::Vector2f& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    inline float LengthSq(const sf::Vector2f& v) {
        return v.x * v.x + v.y * v.y;
    }

    inline float Distance(const sf::Vector2f& a, const sf::Vector2f& b) {
        return Length(b - a);
    }

    inline float DistanceSq(const sf::Vector2f& a, const sf::Vector2f& b) {
        return LengthSq(b - a);
    }

    inline sf::Vector2f Normalize(const sf::Vector2f& v) {
        float len = Length(v);
        if (len < 1e-6f) return sf::Vector2f(0.0f, 0.0f);
        return v / len;
    }

    inline float ToRadians(float degrees) { return degrees * PI / 180.0f; }
    inline float ToDegrees(float radians) { return radians * 180.0f / PI; }

    inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }

    inline sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t) {
        return a + (b - a) * t;
    }

    inline sf::Vector2f DirectionFromAngle(float degrees) {
        float rad = ToRadians(degrees);
        return sf::Vector2f(std::cos(rad), std::sin(rad));
    }

    inline float Clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
} // namespace math
