#pragma once

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "rtweekend.h"
#include "color.h"
#include "material.h"

#include "tracy/Tracy.hpp"

class Camera
{
public:
	static Camera init(
		const vec3& lookFrom,
		const vec3& lookAt,
		const vec3& worldUp,
		float vfov, float focusDist, float defocusAngle, 
		int imageWidth, int imageHeight, float aspectRatio)
	{
		const vec3 center = lookFrom;
		const float theta = degreesToRadians(vfov);
		const float h = std::tan(theta / 2.0);
		const float viewportHeight = 2.0 * h * focusDist;
		const float viewportWidth = viewportHeight * aspectRatio;

		const vec3 front = normalize(lookFrom - lookAt);
		const vec3 right = normalize(cross(worldUp, front));
		const vec3 up = normalize(cross(front, right));

		const vec3 viewportU = viewportWidth * right;
		const vec3 viewportV = -viewportHeight * up; 
		
		const vec3 pixelDeltaU = viewportU / imageWidth;
		const vec3 pixelDeltaV = viewportV / imageHeight;

		const vec3 viewportUpperLeft = center - (focusDist * front) - 0.5 * (viewportU + viewportV);
		const vec3 pixel00 = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
		
		const float defocusRadius = focusDist * std::tan(degreesToRadians(defocusAngle / 2.0));
		const vec3 defocusDiskU = right * defocusRadius;
		const vec3 defocusDiskV = up * defocusRadius;

		return Camera(center, pixel00, pixelDeltaU, pixelDeltaV, defocusDiskU, defocusDiskV, defocusAngle);
	}

	ray getRay(int i, int j) const
	{
		auto offset{ sampleSquare() };

		auto pixelSample{ 
			pixel00 
			+ (i + offset.x()) * pixelDeltaU 
			+ (j + offset.y()) * pixelDeltaV 
		};

		vec3 rayOrigin{ (defocusAngle <= 0.) ? center : defocusDiskSample() };
		auto rayDir{ pixelSample - rayOrigin };

		return ray(rayOrigin, rayDir);
	}

	vec3 center{};
	vec3 pixel00{};
	vec3 pixelDeltaU{};
	vec3 pixelDeltaV{};
	vec3 defocusDiskU{};
	vec3 defocusDiskV{};
	float defocusAngle{};

private:
	Camera(const vec3& center, const vec3& pixel00,
		const vec3& pixelDeltaU, const vec3& pixelDeltaV,
		const vec3& defocusDiskU, const vec3& defocusDiskV,
		float defocusAngle)
	:	center(center), 
		pixel00(pixel00), 
		pixelDeltaU(pixelDeltaU),
		pixelDeltaV(pixelDeltaV),
		defocusDiskU(defocusDiskU), 
		defocusDiskV(defocusDiskV),
		defocusAngle(defocusAngle)
	{
	}

	vec3 defocusDiskSample() const
	{
		vec3 sample{ randomUnitCircle() };

		return center 
			+ defocusDiskU * sample[0]
			+ defocusDiskV * sample[1];
	}
};