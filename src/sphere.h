#pragma once

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

#include "tracy/Tracy.hpp"

class Material;

class Sphere : public Hittable
{
public:
	Sphere(const vec3& center, float radius, Material* material)
		: center{ center }, radius{ std::fmax(0.f, radius) }, mat{ material }
	{
	}

	bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const override // (!) remove?
	{
		return false; 
	}


//private:
	vec3 center{};
	float radius{};
	std::unique_ptr<Material> mat;
};