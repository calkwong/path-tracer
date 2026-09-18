#include "world.h"
#include "hittable.h"
#include "camera.h"

#include <glm/trigonometric.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <cassert>

int main()
{
#ifdef NDEBUG
    std::cout << "Release mode\n";
#else
    std::cout << "Debug mode\n";
#endif

    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1280;
    assert(cam.image_width % 64 == 0 && "Image width not divisible by 64");

    cam.spp = 100;
    cam.max_depth = 20;

    cam.look_from = glm::vec3(13, 2, 3);
    cam.look_at = glm::vec3(0);
    cam.fovy = 20.0f;

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.init();

    // TODO: sort to aid branch prediction?
    World world;
    world.add_sphere(Sphere{ .center = glm::vec3(0, -1000, 0), .radius = 1000, .mat = Material{ .type = MaterialType::Lambertian, .albedo = glm::vec3(0.5) } });
    world.add_sphere(Sphere{ .center = glm::vec3(0, 1, 0), .radius = 1.0, .mat = Material{ .type = MaterialType::Dielectric, .refraction_index = 1.5 } });
    world.add_sphere(Sphere{ .center = glm::vec3(-4, 1, 0), .radius = 1.0, .mat = Material{ .type = MaterialType::Lambertian, .albedo = glm::vec3(0.4, 0.2, 0.1) } });
    world.add_sphere(Sphere{ .center = glm::vec3(4, 1, 0), .radius = 1.0, .mat = Material{ .type = MaterialType::Metal, .albedo = glm::vec3(0.7, 0.6, 0.5) } });

    // Render
    if (cam.render(world))
    {
        return 1;
    }

    return 0;
}
