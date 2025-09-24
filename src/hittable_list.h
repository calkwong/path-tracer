#pragma once

#include "hittable.h"
#include "ray.h"
#include "sphere.h"

#include <vector>
#include <memory>

#include "tracy/Tracy.hpp"

class Sphere;

struct Geometry
{
	vec3 center{};
	float radius{};
};

class HittableList : public Hittable
{
public:
	std::vector<Geometry> spheres{};
	std::vector<std::unique_ptr<Material>> materials{};

	HittableList() {}

	void clear()
	{
		spheres.clear();
		materials.clear(); // (!) leak? 
	}

	void add(Sphere&& s)
	{
		spheres.push_back({ s.center, s.radius });
		materials.push_back(std::move(s.mat));
	}

	bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const override
	{
		float closestSoFar{ tMax };
		int idx{ -1 };

		for (size_t i = 0; i < spheres.size(); i++)
		{
			const vec3 center = spheres[i].center;
			const float radius = spheres[i].radius;

			const vec3 d{ r.direction() };
			const vec3 cq{ center - r.origin() };
			const float a{ d.lengthSquared() };
			const float h{ dot(d, cq) };
			const float c{ cq.lengthSquared() - radius * radius };
			const float discriminant{ h * h - a * c };

			if (discriminant < 0.)
				continue;

			const float sqrtd{ std::sqrt(discriminant) };
			float root{ (h - sqrtd) / a };
			if (root <= tMin || root >= closestSoFar)
			{
				root = (h + sqrtd) / a;
				if (root <= tMin || root >= closestSoFar)
					continue;
			}
			closestSoFar = root;
			idx = i;
		}

		if (idx < 0)
			return false;

		rec.t = closestSoFar;
		rec.point = r.at(rec.t);
		rec.normal = (rec.point - spheres[idx].center) / spheres[idx].radius;
		rec.mat = materials[idx].get();
		
		return true;
	}
};