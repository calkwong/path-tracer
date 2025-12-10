#pragma once

#include "vec3.h"
#include "ray.h"
#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"

#include <thread>
#include <memory>

#include "tracy/Tracy.hpp"

#define MULTITHREADING 1
#define WRITE_COLOR 1

class Application
{
public:

    // CONFIG
    const vec3 lookFrom = vec3(13., 2., 3.);
    const vec3 lookAt = vec3(0.);
    const vec3 worldUp = vec3(0., 1., 0.);
    const int imageWidth = 1200;
    const int imageHeight = 720;
    const float aspectRatio = static_cast<float>(imageWidth) / imageHeight;
    const int pixelSamples = 100;
    const int maxDepth = 20;
    const float vfov = 20.0;
    const float focusDist = 10.0;
    const float defocusAngle = 0.6;

    void init() 
    {
        cam.init(lookFrom, lookAt, worldUp, vfov, focusDist, defocusAngle, imageWidth, imageHeight, aspectRatio);
        initRenderables();
    }

    void render()
    {
        std::vector<vec3> buffer(imageWidth * imageHeight);
    
        std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

    #if MULTITHREADING == 0
        for (int y = 0; y < imageHeight; y++) 
        {
            std::clog << "\rScanlines remaining: " << (imageHeight - y) << ' ' << std::flush;
            for (int x = 0; x < imageWidth; x++) 
            {
                ZoneScoped;
                vec3 pixelColor{};
                for (int sample = 0; sample < pixelSamples; sample++) 
                {
                    ray r = cam.getRay(x, y);
                    pixelColor += rayColor(r, maxDepth, world);
                }
                buffer[y * imageWidth + x] = pixelColor;
            }
        }
        #if WRITE_COLOR == 1
            writeColor(std::cout, pixelColor / static_cast<float>(pixelSamples));
        #endif  
    #elif MULTITHREADING == 1
        const int numThreads = std::thread::hardware_concurrency() - 2;
        std::vector<std::thread> threads{};

        for (int i = 0; i < numThreads; i++)
        {
            threads.emplace_back([&, i]() {
                for (int y = 0; y < imageHeight; y++)
                {
                    for (int x = i; x < imageWidth; x = x + numThreads)
                    {
                        ZoneScoped;
                        vec3 pixelColor{};
                        for (int sample = 0; sample < pixelSamples; sample++)
                        {
                            ray r{ cam.getRay(x, y) }; // (!) not normalized
                            pixelColor += rayColor(r, maxDepth, world);
                            //pixelColor += rayColorNonRecursive(r, maxDepth, world);
                        }
                        buffer[y * imageWidth + x] = pixelColor;
                    }
                }
                });
        }

        for (int t = 0; t < numThreads; t++)
        {
            threads[t].join();
        }
    #endif

    #if WRITE_COLOR == 1
        {
            for (size_t i = 0; i < buffer.size(); i++)
            {
                writeColor(std::cout, buffer[i] / static_cast<float>(pixelSamples));
            }
        }
    #endif

        std::clog << "\rDone.                 \n";
    }

private:
    void initRenderables()
    {
        world.clear();

        world.add(Sphere(vec3(0.f, -1000.f, 0.f), 1000.f, std::make_unique<Lambertian>(vec3(0.5f, 0.5f, 0.5f))));

        for (int a = -11; a < 11; a++)
        {
            for (int b = -11; b < 11; b++)
            {
                auto chooseMat = randomfloat();
                vec3 center(a + 0.9f * randomfloat(), 0.2f, b + 0.9f * randomfloat());

                if ((center - vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
                {
                    if (chooseMat < 0.8f)
                    {
                        // diffuse
                        auto albedo = randomVector();
                        world.add(Sphere(center, 0.2f, std::make_unique<Lambertian>(albedo)));
                    }
                    else if (chooseMat < 0.95f)
                    {
                        // metal
                        auto albedo = randomVector(0.5f, 1.f);
                        world.add(Sphere(center, 0.2f, std::make_unique<Metal>(albedo)));
                    }
                    else
                    {
                        // glass
                        world.add(Sphere(center, 0.2f, std::make_unique<Dielectric>(1.5f)));
                    }
                }
            }
        }

        world.add(Sphere(vec3(0.f, 1.f, 0.f), 1.0f, std::make_unique<Dielectric>(1.5f)));

        world.add(Sphere(vec3(-4.f, 1.f, 0.f), 1.0f, std::make_unique<Lambertian>(vec3(0.4f, 0.2f, 0.1f))));

        world.add(Sphere(vec3(4.f, 1.f, 0.f), 1.0f, std::make_unique<Metal>(vec3(0.7f, 0.6f, 0.5f))));
    }

    vec3 rayColor(const ray& r, int depth, const Hittable& world)
    {
        if (depth <= 0)
            return vec3(1.);

        HitRecord rec{};

        if (world.hit(r, 0.001, infinity, rec)) // account for shadow acne
        {
            ray scattered{};
            vec3 attenuation{};
            if (rec.mat->scatter(r, rec, attenuation, scattered)) // (!) fix unique ptr check
                return attenuation * rayColor(scattered, depth - 1, world);
            return attenuation; // (!) check
        }

        vec3 dir{ normalize(r.direction()) };
        auto a{ 0.5 * (dir.y() + 1.0) };
        return (1.0f - a) * vec3(1.0f) + a * vec3(0.5f, 0.7f, 1.0f);
    }

    vec3 rayColorNonRecursive(ray& r, int depth, const Hittable& world)
    {
        vec3 dir{ normalize(r.direction()) };
        auto a{ 0.5 * (dir.y() + 1.0) };
        vec3 sky = (1.0f - a) * vec3(1.0f) + a * vec3(0.5f, 0.7f, 1.0f);

        vec3 ret{ 1. };
        HitRecord rec{};

        while (true)
        {
            if (depth <= 0)
                return ret;

            if (world.hit(r, 0.001, infinity, rec)) // account for shadow acne
            {
                ray scattered{};
                vec3 attenuation{};
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                {
                    ret *= attenuation;
                    depth -= 1;
                    r = scattered;
                    continue;
                }
                return ret; // (!) no scatter -> absorbed so don't multiply with sky?
            }

            return ret * sky;
        }
    }

    Camera cam{};
    HittableList world{};
};