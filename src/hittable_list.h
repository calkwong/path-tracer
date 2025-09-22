#pragma once

#include "hittable.h"
#include "ray.h"
#include <vector>
#include <memory>

#include "tracy/Tracy.hpp"

class Sphere;

class HittableList : public Hittable
{
public:
	std::vector<std::unique_ptr<Hittable>> objects{};

	HittableList() {}

	void clear()
	{
		objects.clear();
	}

	void add(Hittable* object)
	{
		objects.push_back(std::move(std::unique_ptr<Hittable>(object)));
	}

	bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const override
	{ 
		bool hitAnything{ false };
		auto closestSoFar{ tMax };

		for (const auto& object : objects)
		{
			if (object->hit(r, tMin, closestSoFar, rec))
			{
				hitAnything = true;
				closestSoFar = rec.t;
			}
		}

		return hitAnything;
	}
};