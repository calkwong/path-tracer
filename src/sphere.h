#pragma once

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const vec3& center, double radius)
		: center{ center }, radius{ std::fmax(0, radius) }
	{
	}

	bool hit(const ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		// Section 5.1, 6.2 for proof
		auto d{ r.direction() };
		auto cq{ center - r.origin() };
		auto a{ d.lengthSquared() };
		auto h{ dot(d, cq) };
		auto c{ cq.lengthSquared() - radius * radius };
		auto discriminant{ h * h - a * c };

		if (discriminant < 0)
			return false;

		auto sqrtd{ std::sqrt(discriminant) };
		auto root{ (h - sqrtd) / a }; 
		if (root <= tMin || root >= tMax)
		{
			root = (h + sqrtd) / a;
			if (root <= tMin || root >= tMax)
				return false;
		}

		rec.t = root;
		rec.point = r.at(rec.t);
		auto outwardNormal = (rec.point - center) / radius;
		rec.setFaceNormal(r, outwardNormal);

		return true;
	}

private:
	vec3 center{};
	double radius{};
};

double hitSphere(const vec3 center, double radius, const ray& r)
{
	// Section 5.1, 6.2 for proof
	auto d{ r.direction() };
	auto cq{ center - r.origin() };
	auto a{ d.lengthSquared() };
	auto h{ dot(d, cq) };
	auto c{ cq.lengthSquared() - radius * radius };
	auto discriminant{ h * h - a * c };

	if (discriminant < 0) // no solution
		return -1.0;
	else
		return (h - std::sqrt(discriminant)) / a; // assume smallest t preferred for now
}