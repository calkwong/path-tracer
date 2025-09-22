#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"

#include <thread>
#include <memory>

#include <algorithm>
#include <execution>
#include <ranges>

#include "tracy/Tracy.hpp"

#define MULTITHREADING 1 // SINGLE, 14 NAIVE, 14 TILES, FOR EACH NAIVE
#define WRITE_COLOR 0

vec3 rayColor(const ray& r, int depth, const Hittable& world)
{
    if (depth <= 0)
        return vec3(1.);

    HitRecord rec{};

    if (world.hit(r, 0.001, infinity, rec)) // account for shadow acne
    {
        ray scattered{};
        vec3 attenuation{};
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, depth - 1, world);
        return vec3(0.0f);
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
            return ret * sky; // (!) with or without sky?
        }

        return ret * sky;
    }
}

void render(std::vector<vec3>& buffer, const Hittable& world, const Camera& cam, int imageWidth, int imageHeight, int pixelSamples, int maxDepth, const vec3& center)
{
    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

#if MULTITHREADING == 0
    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            ZoneScoped;
    		vec3 pixel_color{};
            for (int sample = 0; sample < pixelSamples; sample++) {
                ray r = cam.getRay(i, j);
                pixel_color += rayColor(r, maxDepth, world);
            }
        #if WRITE_COLOR == 1
            writeColor(std::cout, pixel_color / static_cast<float>(pixelSamples));
        #endif  
        }
    }
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
                    //buffer[y * imageWidth + x] = pixelColor / static_cast<float>(pixelSamples);
                    buffer[y * imageWidth + x] = pixelColor;
                }
            }
        });
    }

    for (int t = 0; t < numThreads; t++)
    {

        threads[t].join();
    }
#elif MULTITHREADING == 2
    for (int i = 0; i < numThreads; i++)
    {
        threads.emplace_back([&, i]() {
            for (int y = 0; y < imageHeight; y = y + 8)
            {
                for (int x = 8 * i; x < imageWidth; x = x + (8 * numThreads))
                {
                    for (int y2 = 0; y2 < 8; y2++)
                    {
                        for (int t = 0; t < 8; t++)
                        {
                            //ZoneScoped;
                            vec3 pixelColor{};
                            for (int sample = 0; sample < pixelSamples; sample++)
                            {
                                ray r{ cam.getRay(x + t, y + y2) }; // (!) not normalized
                                //pixelColor += rayColor(r, maxDepth, world);
                                pixelColor += rayColorNonRecursive(r, maxDepth, world);
                            }
                            buffer[(y + y2) * imageWidth + (x + t)] = pixelColor / static_cast<float>(pixelSamples);
                        }
                    }
                }
            }
        });
    }

    for (int t = 0; t < numThreads; t++)
    {

        threads[t].join();
    }

#else
    std::vector<int> xs{};
    std::vector<int> ys{};
    for (int i = 0; i < imageWidth; i++)
    {
        xs.push_back(i);
    }
    for (int i = 0; i < imageHeight; i++)
    {
        ys.push_back(i);
    }
    auto tuples = std::views::cartesian_product(xs, ys);
     
    std::for_each(std::execution::par, tuples.begin(), tuples.end(),
        [&](const auto& pair) {
            ZoneScoped;
            const auto& [x, y] = pair;
            vec3 pixelColor{};

            for (int sample = 0; sample < pixelSamples; sample++)
            {
                ray r{ cam.getRay(x, y) }; // (!) not normalized
                //pixelColor += rayColor(r, maxDepth, world);
                pixelColor += rayColorNonRecursive(r, maxDepth, world);
            }
            buffer[y * imageWidth + x] = pixelColor / static_cast<float>(pixelSamples);
        });

#endif

#if WRITE_COLOR == 1
    {
        for (size_t i = 0; i < buffer.size(); i++)
        {
            writeColor(std::cout, buffer[i]);
        }
    }
#endif

    std::clog << "\rDone.                 \n";
}

int main() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // establish connection with tracy
    ZoneScoped;

    HittableList world{};

    {
        auto groundMaterial = new Lambertian(vec3(0.5f, 0.5f, 0.5f));
        world.add(new Sphere(vec3(0.f, -1000.f, 0.f), 1000.f, groundMaterial));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = randomfloat();
                vec3 center(a + 0.9f * randomfloat(), 0.2f, b + 0.9f * randomfloat());

                if ((center - vec3(4.f, 0.2f, 0.f)).length() > 0.9f) {
                    Material* sphereMaterial;

                    if (choose_mat < 0.8f) {
                        // diffuse
                        auto albedo = randomVector();
                        sphereMaterial = new Lambertian(albedo);
                        world.add(new Sphere(center, 0.2f, sphereMaterial));
                    }
                    else if (choose_mat < 0.95f) {
                        // metal
                        auto albedo = randomVector(0.5f, 1.f);
                        sphereMaterial = new Metal(albedo);
                        world.add(new Sphere(center, 0.2f, sphereMaterial));
                    }
                    else {
                        // glass
                        sphereMaterial = new Dielectric(1.5f);
                        world.add(new Sphere(center, 0.2f, sphereMaterial));
                    }
                }
            }
        }

        auto material1 = new Dielectric(1.5f);
        world.add(new Sphere(vec3(0.f, 1.f, 0.f), 1.0f, material1));

        auto material2 = new Lambertian(vec3(0.4f, 0.2f, 0.1f));
        world.add(new Sphere(vec3(-4.f, 1.f, 0.f), 1.0f, material2));

        auto material3 = new Metal(vec3(0.7f, 0.6f, 0.5f));
        world.add(new Sphere(vec3(4.f, 1.f, 0.f), 1.0f, material3));
    }

    const vec3 lookFrom = vec3(13., 2., 3.);
    const vec3 lookAt = vec3(0.);
    const vec3 worldUp = vec3(0., 1., 0.);

    const int imageWidth = 1200;
    const int imageHeight = 720;
    const float aspectRatio = static_cast<float>(imageWidth) / imageHeight;
    const int pixelSamples = 100;
    const int maxDepth = 10;
    const float vfov = 20.0;
    const float focusDist = 10.0;
    const float defocusAngle = 0.6;

    const Camera cam = Camera::init(lookFrom, lookAt, worldUp, vfov, focusDist, defocusAngle,
        imageWidth, imageHeight, aspectRatio);

    std::vector<vec3> buffer(imageHeight * imageWidth);

    render(buffer, world, cam, imageWidth, imageHeight, pixelSamples, maxDepth, lookFrom);

    return 0;
}