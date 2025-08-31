#pragma once

#include "vec3.h"
#include "ray.h"
#include "rtweekend.h"
#include "hittable.h"

class Material
{
public:
	Material(const vec3& albedo, double metallic)
		: albedo{ albedo }, metallic{ metallic }
	{
	}

	bool scatter(const ray& r, const HitRecord& rec, vec3& attenuation, ray& scattered) const
	{
		if (metallic)
		{
			vec3 reflected{ reflect(r.direction(), rec.normal) };
			scattered = ray(rec.point, reflected);
		}
		else
		{
			auto scatterDirection{ rec.normal + randomUnitVector(-1, 1) };

			if (scatterDirection.nearZero())
				scatterDirection = rec.normal;

			scattered = ray(rec.point, scatterDirection);
		}

		attenuation = albedo;
		return true;
	}

private:
	vec3 albedo{};
	double metallic{};
};