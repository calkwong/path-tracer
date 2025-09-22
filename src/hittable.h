#pragma once

#include "vec3.h"
#include "ray.h"

class Material;

class HitRecord
{
public:
	vec3 point{};
	vec3 normal{};
	Material* mat{};
	float t{}; // computed from root
};

class Hittable
{
public:

	Hittable() = default;
	Hittable(const Hittable&) = default;
	Hittable(Hittable&&) = default;
	Hittable& operator=(const Hittable&) = default;
	Hittable& operator=(Hittable&&) = default;
	virtual ~Hittable() = default;

	virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};