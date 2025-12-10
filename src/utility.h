#pragma once

#include <cmath>
#include <limits>
#include <random>
#include <cstdlib>
#include <iostream>

#include "vec3.h"

const float infinity{ std::numeric_limits<float>::infinity() };
const float pi = 3.1415926535897932385;

inline float degreesToRadians(float degrees)
{
	return degrees * pi / 180.0f;
}

inline float linearToGamma(float a)
{
    auto gamma{ 2.2f };
    return std::pow(a, 1.0f / gamma);
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2.0 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
	auto cos_theta = std::fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.lengthSquared())) * n;
	return r_out_perp + r_out_parallel;
}

// color is [0, 1]
inline void writeColor(std::ostream& out, const vec3& color)
{
	auto r{ color.x() };
	auto g{ color.y() };
	auto b{ color.z() };

	r = linearToGamma(r);
	g = linearToGamma(g);
	b = linearToGamma(b);

	int ir{ static_cast<int>(r * 255.999) };
	int ig{ static_cast<int>(g * 255.999) };
	int ib{ static_cast<int>(b * 255.999) };

	out << ir << ' ' << ig << ' ' << ib << '\n';
}