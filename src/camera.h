#pragma once

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "rtweekend.h"
#include "color.h"

class Camera
{
public:
	double aspectRatio{ 1.0 };
	int imageWidth{ 100 };
	int pixelSamples{ 10 };
	int maxDepth{ 100 };

	Camera() {}

	void render(const Hittable& world)
	{
		initialize();

		std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++)
		{
			std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
			for (int i = 0; i < imageWidth; i++)
			{
				vec3 pixelColor{};
				for (int sample = 0; sample < pixelSamples; sample++)
				{
					ray r{ getRay(i, j) };
					pixelColor += rayColor(r, maxDepth, world);
				}
				writeColor(std::cout, pixelColor * pixelSampleScale);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	int imageHeight{};
	vec3 center{};
	vec3 pixel00{};
	vec3 pixelDeltaU{};
	vec3 pixelDeltaV{};
	double pixelSampleScale{};

	void initialize()
	{
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = imageHeight < 1 ? 1 : imageHeight;
		pixelSampleScale = 1.0 / pixelSamples;

		auto focalLength{ 1.0 };
		auto viewportHeight{ 2.0 };
		auto viewportWidth{ viewportHeight * imageWidth / imageHeight };

		auto viewportU{ vec3(viewportWidth, 0, 0) };
		auto viewportV{ vec3(0, -viewportHeight, 0) };

		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		auto viewportUpperLeft{ center - vec3(0, 0, focalLength) - 0.5 * (viewportU + viewportV) };
		pixel00 = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
	}

	vec3 rayColor(const ray& r, int depth, const Hittable& world)
	{
		if (depth <= 0)
			return vec3(0);

		HitRecord rec{};

		if (world.hit(r, 0.001, infinity, rec)) // account for shadow acne
		{
			vec3 unitVec{ randomUnitVector(-1, 1) };
			auto scatter{ unitVec + rec.normal }; // not normalized but acceptable, max length of 2
			return 0.5 * rayColor(ray(rec.point, scatter), depth - 1, world);
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
		auto offset{ sampleSquare() };
		auto pixelSample{ pixel00 + (i + offset.x()) * pixelDeltaU + (j + offset.y()) * pixelDeltaV };
		auto rayDir{ pixelSample - center };

		return ray(center, rayDir);
	}
};