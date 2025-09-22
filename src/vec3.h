#pragma once

#include "rtweekend.h"
#include <cmath>

class vec3
{
public:
	//std::array<float, 3> data{};
	float data[3]{};

	vec3() {}
	vec3(float x, float y, float z) : data(x, y, z){}
	vec3(float x) : data(x, x, x) {}

	float x() const { return data[0]; }
	float y() const { return data[1]; }
	float z() const { return data[2]; }

	vec3 operator-() const { return vec3(-data[0], -data[1], -data[2]); }
	float operator[](int i) const { return data[i]; }

	vec3& operator+=(const vec3& v)
	{
		data[0] += v[0];
		data[1] += v[1];
		data[2] += v[2];

		return *this;
	}

	vec3& operator-=(const vec3& v)
	{
		data[0] -= v[0];
		data[1] -= v[1];
		data[2] -= v[2];

		return *this;
	}

	vec3& operator*=(float t)
	{
		data[0] *= t;
		data[1] *= t;
		data[2] *= t;

		return *this;
	}

	vec3& operator*=(const vec3& v)
	{
		data[0] *= v[0];
		data[1] *= v[1];
		data[2] *= v[2];

		return *this;
	}

	vec3& operator/=(float t)
	{
		return *this *= 1 / t;
	}

	float length() const
	{
		return std::sqrt(lengthSquared());
	}

	float lengthSquared() const
	{
		return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
	}

	bool nearZero() const
	{
		auto threshold{ 1e-8 };
		return (std::fabs(data[0]) < threshold) && (std::fabs(data[1]) < threshold) && (std::fabs(data[2]) < threshold);
	}
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) 
{
	return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) 
{
	return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) 
{
	return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(const vec3& v, float t) 
{
	return v * vec3(t);
}

inline vec3 operator*(float t, const vec3& v) 
{
	return v * t;
}

inline vec3 operator/(const vec3& v, float t)
{
	return v * (1 / t);
}

inline float dot(const vec3& u, const vec3& v)
{
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(
		u[1] * v[2] - u[2] * v[1],
		u[2] * v[0] - u[0] * v[2],
		u[0] * v[1] - u[1] * v[0]
	);
}

inline vec3 normalize(const vec3& v)
{
	return v / v.length();
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

inline vec3 sampleSquare()
{
	return vec3(randomfloat() - 0.5, randomfloat() - 0.5, 0.);
}