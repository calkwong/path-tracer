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

	auto material_ground = std::make_shared<Material>(vec3(0.8, 0.8, 0.0), 0.0);
	auto material_center = std::make_shared<Material>(vec3(0.1, 0.2, 0.5), 0.0);
	auto material_left = std::make_shared<Material>(vec3(0.8, 0.8, 0.8), 1.0, 1.5, true);
	auto material_bubble = std::make_shared<Material>(vec3(0.8, 0.8, 0.8), 1.0, 1.0 / 1.5, true);
	auto material_right = std::make_shared<Material>(vec3(0.8, 0.6, 0.2), 1.0);

	world.add(std::make_shared<Sphere>(vec3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(std::make_shared<Sphere>(vec3(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(std::make_shared<Sphere>(vec3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(std::make_shared<Sphere>(vec3(-1.0, 0.0, -1.0), 0.4, material_bubble));
	world.add(std::make_shared<Sphere>(vec3(1.0, 0.0, -1.0), 0.5, material_right));

	Camera cam{};
	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.pixelSamples = 10;
	cam.maxDepth = 50;

	cam.vfov = 90;
	cam.lookFrom = vec3(-2, 2, 1);
	cam.lookAt = vec3(0, 0, -1);

	cam.defocusAngle = 10.0;
	cam.focusDist = 3.4;

	cam.render(world);

	return 0;
}