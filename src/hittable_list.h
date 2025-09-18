#pragma once

#include "hittable.h"
#include "ray.h"
#include <vector>
#include <memory>

#include "sphere.h"

class Sphere;

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Sphere>> objects{};

	HittableList() {}

	HittableList(std::shared_ptr<Sphere> object)
	{
		add(object);
	}

	void clear()
	{
		objects.clear();
	}

	void add(std::shared_ptr<Sphere> object)
	{
		objects.push_back(object);
	}

	bool hit(const ray& r, double tMin, double tMax, HitRecord& rec) const
	{ 
		HitRecord tempRec{};
		bool hitAnything{ false };
		auto closestSoFar{ tMax };

		for (const auto& object : objects)
		{
			if (object->hit(r, tMin, closestSoFar, tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.t;
				rec = tempRec; // (!) redundant?
			}
		}

		return hitAnything;
	}
};