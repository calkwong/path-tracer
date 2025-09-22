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

	bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const override
	{
		// Section 5.1, 6.2 for proof
		const vec3 d{ r.direction() };
		const vec3 cq{ center - r.origin() };
		const float a{ d.lengthSquared() };
		const float h{ dot(d, cq) };
		const float c{ cq.lengthSquared() - radius * radius };
		const float discriminant{ h * h - a * c };

		if (discriminant < 0.)
			return false;

		const float sqrtd{ std::sqrt(discriminant) };
		float root{ (h - sqrtd) / a }; 
		if (root <= tMin || root >= tMax)
		{
			root = (h + sqrtd) / a;
			if (root <= tMin || root >= tMax)
				return false;
		}

		rec.t = root;
		rec.point = r.at(rec.t);
		rec.normal = (rec.point - center) / radius; // (!) always normalized
		rec.mat = mat.get();

		return true;
	}

private:
	vec3 center{};
	float radius{};
	std::unique_ptr<Material> mat;
};