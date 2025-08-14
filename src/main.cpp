#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

vec3 rayColor(const ray& r)
{
	vec3 dir{ normalize(r.direction()) };
	auto a{ 0.5f * (dir.y() + 1.0f) };
	return (1.0f - a) * vec3(1.0f) + a * vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	// image
	auto aspectRatio{ 16.0f / 9.0f };
	int imageWidth{ 400 };
	int imageHeight{ int(imageWidth / aspectRatio) };
	imageHeight = imageHeight < 1 ? 1 : imageHeight;

	// camera
	auto focalLength{ 1.0f };
	auto viewportHeight{ 2.0f };
	auto viewportWidth{ viewportHeight * imageWidth / imageHeight }; // (!)
	auto cameraCenter{ vec3(0) };

	auto viewportU{ vec3(viewportWidth, 0, 0) };
	auto viewportV{ vec3(0, -viewportHeight, 0) };

	auto pixelDeltaU{ viewportU / imageWidth };
	auto pixelDeltaV{ viewportV / imageHeight };

	auto viewportUpperLeft{ cameraCenter - vec3(0, 0, focalLength) - 0.5f * (viewportU + viewportV) };
	auto pixel00{ viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV) };

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