#include "world.h"
#include "hittable.h"
#include "camera.h"
#include "rando.h"

#include <glm/trigonometric.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <algorithm>

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

    cam.spp = 500;
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

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random_float();
            glm::vec3 center{ a + 0.9 * random_float(), 0.2, b + 0.9 * random_float() };

            if ((center - glm::vec3{ 4, 0.2, 0 }).length() > 0.9)
            {
                if (choose_mat < 0.8)
                {
                    // diffuse
                    Material mat = { .type = MaterialType::Lambertian, .albedo = random_vector() };
                    Sphere sphere = { .center = center, .radius = 0.2, .mat = mat };
                    world.add_sphere(sphere);
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    Material mat = { .type = MaterialType::Metal, .albedo = random_vector(0.5, 1.0) };
                    Sphere sphere = { .center = center, .radius = 0.2, .mat = mat };
                    world.add_sphere(sphere);
                }
                else
                {
                    // glass
                    Material mat = { .type = MaterialType::Dielectric, .refraction_index = 1.5 };
                    Sphere sphere = { .center = center, .radius = 0.2, .mat = mat };
                    world.add_sphere(sphere);
                }
            }
        }
    }

    // Doesn't seem to make a difference
    std::sort(world.spheres.begin(), world.spheres.end(), [](const Sphere& a, const Sphere& b)
              {
                  return a.mat.type < b.mat.type;
              });

    // Render
    if (cam.render(world))
    {
        return 1;
    }

    return 0;
}
