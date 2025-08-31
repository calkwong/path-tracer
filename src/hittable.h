#pragma once

#include "vec3.h"
#include "ray.h"

class Material;

class HitRecord
{
public:
	vec3 point{};
	vec3 normal{};
	std::shared_ptr<Material> mat{};
	double t{}; // computed from root
	bool frontFace{};

	void setFaceNormal(const ray& r, const vec3& outwardNormal)
	{
		frontFace = dot(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};