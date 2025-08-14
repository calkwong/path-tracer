#include <iostream>
#include "vec3.h"
#include "color.h"

int main()
{
	// image
	constexpr int imageWidth{ 256 };
	constexpr int imageHeight{ 256 };

	// render
	std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	bool stop = false;

	for (int i = 0; i < imageHeight; i++)
	{
		std::clog << "\rScanlines remaining: " << (imageHeight - i) << ' ' << std::flush;
		for (int j = 0; j < imageWidth; j++)
		{
			writeColor(std::cout, vec3(j, i, 0));
		}
	}

	std::clog << "\rDone.                 \n";

	return 0;
}