#include "interval.h"

auto Interval::size() const -> float
{
    return max - min;
}

auto Interval::contains(float x) const -> bool
{
    return min <= x && x <= max;
}

auto Interval::surrounds(float x) const -> bool
{
    return min < x && x < max;
}

auto Interval::empty() -> Interval
{
    return Interval{ INF, -INF };
}

auto Interval::universe() -> Interval
{
    return Interval{ -INF, INF };
}

auto Interval::clamp(float x) const -> float
{
    if (x < min)
    {
        return min;
    }
    if (x > max)
    {
        return max;
    }
    return x;
}
