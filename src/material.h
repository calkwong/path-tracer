#pragma once

#include "vec3.h"
#include "ray.h"
#include "rtweekend.h"
#include "hittable.h"

#include "tracy/Tracy.hpp"

class Material
{
public:
	Material(const vec3& albedo, double metallic, double refractiveIdx = 1.0, bool dielectric = false)
		: albedo{ albedo }, metallic{ metallic }, refractiveIdx{ refractiveIdx }, dielectric{ dielectric }
	{
	}

	bool scatter(const ray& r, const HitRecord& rec, vec3& attenuation, ray& scattered)
	{
		ZoneScoped;
		vec3 direction{};

		if (!dielectric)
		{
			attenuation = albedo;

			if (metallic)
			{
				direction = reflect(r.direction(), rec.normal);
			}
			else
			{
				direction = rec.normal + randomUnitVector(-1, 1);

				if (direction.nearZero())
					direction = rec.normal; // (!) potential minor bias towards normal?
			}
		}
		else
		{
			attenuation = vec3(1.0);
			double eta = rec.frontFace ? (1.0 / refractiveIdx) : refractiveIdx;

			auto unitDir{ normalize(r.direction()) }; // (!) necessary to normalize
			double cosTheta{ std::fmin(dot(-unitDir, rec.normal), 1.0) };
			double sinTheta{ std::sqrt(1.0 - cosTheta * cosTheta) };
			bool cannotRefract{ (eta * sinTheta) > 1.0 };

			if (cannotRefract)
				direction = reflect(unitDir, rec.normal);
			else
				direction = refract(unitDir, rec.normal, eta);

		}

		scattered = ray(rec.point, direction);

		return true;
	}

private:
	vec3 albedo{};
	double metallic{};
	double refractiveIdx{};
	bool dielectric{ };
};