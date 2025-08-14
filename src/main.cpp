#include <iostream>
#include "vec3.h"

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
			auto r{ static_cast<float>(j) / (imageWidth - 1) };
			auto g{ static_cast<float>(i) / (imageHeight - 1) };
			auto b{ 0.0f };

			int ir{ static_cast<int>(r * 256) };
			int ig{ static_cast<int>(g * 256) };
			int ib{ static_cast<int>(b * 256) };

			writeColor(std::cout, vec3(ir, ig, ib));
		}
	}

	std::clog << "\rDone.                 \n";

	return 0;
}