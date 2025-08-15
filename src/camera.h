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

	Camera() {}

	void render(const Hittable& world)
	{
		initialize();

		std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int i = 0; i < imageHeight; i++)
		{
			std::clog << "\rScanlines remaining: " << (imageHeight - i) << ' ' << std::flush;
			for (int j = 0; j < imageWidth; j++)
			{
				auto pixelCenter{ pixel00 + j * pixelDeltaU + i * pixelDeltaV };
				auto rayDir{ pixelCenter - center };

				ray r(center, rayDir);
				vec3 pixelColor{ rayColor(r, world) };

				writeColor(std::cout, pixelColor);
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

	void initialize()
	{
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = imageHeight < 1 ? 1 : imageHeight;

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

	vec3 rayColor(const ray& r, const Hittable& world)
	{
		HitRecord rec{};

		if (world.hit(r, 0, infinity, rec))
			return 0.5 * (rec.normal + vec3(1));

		vec3 dir{ normalize(r.direction()) };
		auto a{ 0.5 * (dir.y() + 1.0) };
		return (1.0 - a) * vec3(1.0) + a * vec3(0.5, 0.7, 1.0);
	}

};