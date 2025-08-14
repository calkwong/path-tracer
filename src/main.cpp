#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

bool hitSphere(const vec3 center, double radius, const ray& r)
{
	auto d{ r.direction() };
	auto cq{ center - r.origin() };
	auto a{ dot(d, d) };
	auto b{ dot(-2 * d, cq) };
	auto c{ dot(cq, cq) - radius * radius };
	auto discriminant{ b * b - 4 * a * c };

	return (discriminant >= 0);
}

vec3 rayColor(const ray& r)
{
	if (hitSphere(vec3(0, 0, -1), 0.5, r))
		return vec3(1, 0, 0);

	vec3 dir{ normalize(r.direction()) };
	auto a{ 0.5 * (dir.y() + 1.0) };
	return (1.0 - a) * vec3(1.0) + a * vec3(0.5, 0.7, 1.0);
}

int main()
{
	// image
	auto aspectRatio{ 16.0 / 9.0 };
	int imageWidth{ 400 };
	int imageHeight{ int(imageWidth / aspectRatio) };
	imageHeight = imageHeight < 1 ? 1 : imageHeight;

	// camera
	auto focalLength{ 1.0 };
	auto viewportHeight{ 2.0 };
	auto viewportWidth{ viewportHeight * imageWidth / imageHeight }; // (!)
	auto cameraCenter{ vec3(0) };

	auto viewportU{ vec3(viewportWidth, 0, 0) };
	auto viewportV{ vec3(0, -viewportHeight, 0) };

	auto pixelDeltaU{ viewportU / imageWidth };
	auto pixelDeltaV{ viewportV / imageHeight };

	auto viewportUpperLeft{ cameraCenter - vec3(0, 0, focalLength) - 0.5 * (viewportU + viewportV) };
	auto pixel00{ viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV) };

	// render
	std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	for (int i = 0; i < imageHeight; i++)
	{
		std::clog << "\rScanlines remaining: " << (imageHeight - i) << ' ' << std::flush;
		for (int j = 0; j < imageWidth; j++)
		{
			auto pixelCenter{ pixel00 + j * pixelDeltaU + i * pixelDeltaV};
			auto rayDir{ pixelCenter - cameraCenter };

			ray r(cameraCenter, rayDir);
			vec3 pixelColor{ rayColor(r) };

			writeColor(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone.                 \n";

	return 0;
}