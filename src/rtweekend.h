#pragma once

#include <cmath>
#include <limits>
#include <random>
#include "vec3.h"

const double infinity{ std::numeric_limits<double>::infinity() };
const double pi{ 3.1415926535897932385 };

inline double degreesToRadians(double degrees)
{
	return degrees * pi / 180.0;
}

inline double randomDouble() 
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0); // [0.0, 1.0)
    static std::mt19937 generator{};
    return distribution(generator);
}

inline double randomDouble(double min, double max)
{
    return min + (max - min) * randomDouble();
}

inline vec3 randomVector()
{
    auto v{ vec3(randomDouble(), randomDouble(), randomDouble()) };
}

inline vec3 randomVector(double min, double max)
{
    auto v{ vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)) };
}

inline vec3 randomUnitVector()
{
    while (true)
    {
        auto v{ vec3(randomDouble(), randomDouble(), randomDouble()) };
        auto lensq{ v.lengthSquared() };
        if (lensq > 1e-160)
            return v / std::sqrt(lensq);
    }
}

inline vec3 randomUnitVector(double min, double max)
{
    while (true)
    {
        auto v{ vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)) };
        auto lensq{ v.lengthSquared() };
        if (lensq > 1e-160)
            return v / std::sqrt(lensq);
    }
}