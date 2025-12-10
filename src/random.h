#pragma once

#include <cmath>
#include <limits>
#include <random>
#include <cstdlib>

#include "vec3.h"

inline float randomfloat() 
{
    thread_local std::uniform_real_distribution<float> distribution; // [0.0, 1.0)
    thread_local std::mt19937 generator;
    return distribution(generator);
}

inline float randomfloat(float min, float max)
{
    return min + (max - min) * randomfloat();
}

inline vec3 randomVector()
{
	return vec3(randomfloat(), randomfloat(), randomfloat());
}

inline vec3 randomVector(float min, float max)
{
	return vec3(randomfloat(min, max), randomfloat(min, max), randomfloat(min, max));
}

inline vec3 randomUnitVector()
{
	while (true)
	{
		auto v{ randomVector(-1, 1) };
		auto lensq{ v.lengthSquared() };
		if (lensq > 1e-160 && lensq <= 1.0)
			return v / std::sqrt(lensq);
	}
}

inline vec3 randomUnitCircle()
{
	while (true)
	{
		vec3 v{ randomfloat(-1.f, 1.f), randomfloat(-1.f, 1.f), 0.f };
		if (v.lengthSquared() < 1.f)
			return v;
	}
}

inline vec3 sampleSquare()
{
	return vec3(randomfloat() - 0.5, randomfloat() - 0.5, 0.);
}