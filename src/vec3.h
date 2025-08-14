#pragma once

#include <cmath>
#include <array>
#include <iostream>

class vec3
{
public:
	std::array<float, 3> data{};

	vec3() : data{}
	{
	}

	vec3(float x, float y, float z) : data{ x, y, z }
	{
	}

	vec3(float x) : data{ x, x, x }
	{
	}

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

inline void print(const vec3& v)
{
	std::cout << v[0] << ' ' << v[1] << ' ' << v[2] << '\n';
}
