#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <memory>

int main()
{
	HittableList world{};
	world.add(std::make_shared<Sphere>(vec3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(vec3(0, -100.5, -1), 100));

	Camera cam{};
	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;

	cam.render(world);

	return 0;
}