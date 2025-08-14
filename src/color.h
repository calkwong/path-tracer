#pragma once

#include "vec3.h"
#include <iostream>

// color is [0, 1]
inline void writeColor(std::ostream& out, const vec3& color)
{
	auto r{ color.x() };
	auto g{ color.y() };
	auto b{ color.z() };

	int ir{ static_cast<int>(r * 256) };
	int ig{ static_cast<int>(g * 256) };
	int ib{ static_cast<int>(b * 256) };

	out << color[0] << ' ' << color[1] << ' ' << color[2] << '\n';
}
