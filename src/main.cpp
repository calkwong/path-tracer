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

#include "tracy/Tracy.hpp"

#include <chrono>

#define METALLIC 1.0
#define LAMBERTIAN 0.0
#define DIELECTRIC 1.0

int main() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // establish connection with tracy
    ZoneScoped;

    HittableList world;

    vec3 defaultAlbedo{};

    auto ground_material = std::make_shared<Material>(vec3(0.5, 0.5, 0.5), LAMBERTIAN);
    world.add(std::make_shared<Sphere>(vec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> Sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = randomVector();
                    Sphere_material = std::make_shared<Material>(albedo, LAMBERTIAN);
                    world.add(std::make_shared<Sphere>(center, 0.2, Sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = randomVector(0.5, 1);
                    Sphere_material = std::make_shared<Material>(albedo, METALLIC);
                    world.add(std::make_shared<Sphere>(center, 0.2, Sphere_material));
                }
                else {
                    // glass
                    Sphere_material = std::make_shared<Material>(defaultAlbedo, LAMBERTIAN, 1.5, DIELECTRIC);
                    world.add(std::make_shared<Sphere>(center, 0.2, Sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Material>(defaultAlbedo, LAMBERTIAN, 1.5, DIELECTRIC);
    world.add(std::make_shared<Sphere>(vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Material>(vec3(0.4, 0.2, 0.1), LAMBERTIAN);
    world.add(std::make_shared<Sphere>(vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Material>(vec3(0.7, 0.6, 0.5), METALLIC);
    world.add(std::make_shared<Sphere>(vec3(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imageWidth = 1200;
    cam.pixelSamples = 100;
    cam.maxDepth = 50;

    cam.vfov = 20;
    cam.lookFrom = vec3(13, 2, 3);
    cam.lookAt = vec3(0, 0, 0);

    cam.defocusAngle = 0.6;
    cam.focusDist = 10.0;

    cam.render(world);

    return 0;
}