#pragma once

#include <limits>

inline constexpr auto INF = std::numeric_limits<float>::infinity();

struct Interval
{
    float min;
    float max;

    auto size() const -> float;
    auto contains(float x) const -> bool;
    auto surrounds(float x) const -> bool;
    auto clamp(float x) const -> float;

    static auto empty() -> Interval;
    static auto universe() -> Interval;
};
