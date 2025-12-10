#pragma once

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "utility.h"
#include "random.h"

#include "tracy/Tracy.hpp"

class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(const ray& r, const HitRecord& rec, vec3& attenuation, ray& scattered) const
	{
		return false;
	};
};


class Metal final: public Material
{
public:
	Metal(const vec3& albedo)
		: albedo(albedo)
	{
	}

	bool scatter(const ray& r, const HitRecord& rec, vec3& attenuation, ray& scattered) const override
	{
		vec3 direction = reflect(r.direction(), rec.normal);
		scattered = ray(rec.point, direction);
		attenuation = albedo;

		//return true;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	vec3 albedo{};
};

class Lambertian final: public Material
{
public:
	Lambertian(const vec3& albedo)
		: albedo(albedo)
	{
	}

	bool scatter(const ray& r, const HitRecord& rec, vec3& attenuation, ray& scattered) const override
	{
		vec3 direction = rec.normal + randomUnitVector();

		if (direction.nearZero())
			direction = rec.normal; // (potential bias)

		scattered = ray(rec.point, direction);
		attenuation = albedo;

		return true;
	}

private:
	vec3 albedo{};
};

class Dielectric final : public Material
{
public:
	Dielectric(float refractiveIdx)
		: refractiveIdx(refractiveIdx)
	{
	}

	bool scatter(const ray& r, const HitRecord& rec, vec3& attenuation, ray& scattered) const override
	{
		attenuation = vec3(1.f);

		const bool frontFace = dot(rec.normal, r.direction()) < 0.0;
		const float eta = frontFace ? (1.0f / refractiveIdx) : refractiveIdx;
		const vec3 normal = frontFace ? rec.normal : -rec.normal;

		auto unitDir{ normalize(r.direction()) }; // (!) necessary to normalize (!!) 20/9 verify
		float cosTheta{ std::fmin(dot(-unitDir, normal), 1.0f) };
		float sinTheta{ std::sqrt(1.0f - cosTheta * cosTheta) };
		bool cannotRefract{ (eta * sinTheta) > 1.0f };

		vec3 direction{};

		if (cannotRefract || reflectance(cosTheta, eta) > randomfloat())
			direction = reflect(unitDir, normal);
		else
			direction = refract(unitDir, normal, eta);

		scattered = ray(rec.point, direction);

		return true;
	}

private:
	float refractiveIdx{};

	static float reflectance(float cosine, float refraction_index) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};
