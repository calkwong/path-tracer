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
	double aspectRatio{ 1.0 };
	int imageWidth{ 100 };
	int pixelSamples{ 10 };
	int maxDepth{ 10 };
	double vfov{ 90.0 };
	vec3 lookFrom{ vec3(0.) };
	vec3 lookAt{ vec3(0., 0., -1) };
	vec3 worldUp{ vec3(0., 1., 0.) };

	double defocusAngle{};
	double focusDist{ 10.0 };

	Camera() {} // (!) redundant default constructed?

	void render(const Hittable& world)
	{
		initialize();

		std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++)
		{
			FrameMark;
			std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
			for (int i = 0; i < imageWidth; i++)
			{
				vec3 pixelColor{};
				for (int sample = 0; sample < pixelSamples; sample++)
				{
					ray r{ getRay(i, j) }; // (!) not normalized
					pixelColor += rayColor(r, maxDepth, world);
				}
				writeColor(std::cout, pixelColor * pixelSampleScale);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	vec3 center{};
	vec3 pixel00{};
	vec3 pixelDeltaU{};
	vec3 pixelDeltaV{};
	vec3 right{};
	vec3 up{};
	vec3 front{};
	vec3 defocusDiskU{};
	vec3 defocusDiskV{};

	double pixelSampleScale{};
	int imageHeight{};

	void initialize()
	{
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = imageHeight < 1 ? 1 : imageHeight;
		pixelSampleScale = 1.0 / pixelSamples;

		center = lookFrom;

		auto theta{ degreesToRadians(vfov) };
		auto h{ std::tan(theta / 2.0) };
		auto viewportHeight{ 2.0 * h * focusDist};
		auto viewportWidth{ viewportHeight * imageWidth / imageHeight };

		front = normalize(lookFrom - lookAt);
		right = normalize(cross(worldUp, front));
		up = normalize(cross(front, right));

		auto viewportU{ viewportWidth * right };
		auto viewportV{ -viewportHeight * up }; // (!) why negative again? see before positionable-camera

		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		auto viewportUpperLeft{ 
			center 
			- (focusDist * front) 
			- 0.5 * (viewportU + viewportV) 
		};
		pixel00 = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

		double defocusRadius{ focusDist * std::tan(degreesToRadians(defocusAngle / 2.0)) };
		defocusDiskU = right * defocusRadius;
		defocusDiskV = up * defocusRadius;
	}

	vec3 rayColor(const ray& r, int depth, const Hittable& world)
	{
		ZoneScoped;
		if (depth <= 0)
			return vec3(0.0);

		HitRecord rec{};

		if (world.hit(r, 0.001, infinity, rec)) // account for shadow acne
		{
			ray scattered{};
			vec3 attenuation{};
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * rayColor(scattered, depth - 1, world);
			return vec3(0.0);
		}

		vec3 dir{ normalize(r.direction()) };
		auto a{ 0.5 * (dir.y() + 1.0) };
		return (1.0 - a) * vec3(1.0) + a * vec3(0.5, 0.7, 1.0);
	}

	vec3 sampleSquare() const
	{
		return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
	}

	ray getRay(int i, int j) const
	{
		ZoneScoped;
		auto offset{ sampleSquare() };
		auto pixelSample{ 
			pixel00 
			+ (i + offset.x()) * pixelDeltaU 
			+ (j + offset.y()) * pixelDeltaV 
		};
		vec3 rayOrigin{ (defocusAngle <= 0) ? center : defocusDiskSample() };
		auto rayDir{ pixelSample - rayOrigin };

		return ray(rayOrigin, rayDir);
	}

	vec3 defocusDiskSample() const
	{
		vec3 sample{ randomUnitCircle() };

		return center 
			+ defocusDiskU * sample[0]
			+ defocusDiskV * sample[1];
	}
};