#include "camera.h"
#include "glm/trigonometric.hpp"
#include "ray.h"
#include "world.h"
#include "hittable.h"
#include "interval.h"
#include "rando.h"

#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/exponential.hpp>
#include <glm/gtc/random.hpp>
#include <stb_image_write.h>
#include <tracy/Tracy.hpp>

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <execution>
#include <chrono>

namespace
{
auto linear_to_gamma(glm::vec3 color) -> glm::vec3
{
    constexpr auto exponent = glm::vec3(1.0f / 2.2f);
    return glm::pow(color, exponent);
}

auto near_zero(glm::vec3 v) -> bool
{
    auto s = 1e-8f;
    return (glm::abs(v.x) <= s && glm::abs(v.y) <= s && glm::abs(v.z) <= s);
}

// Uses Schlick's approximation for reflectance
auto reflectance(float cos_theta, float refraction_index) -> float
{
    auto r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * glm::pow((1.0 - cos_theta), 5.0f);
}

} // namespace

auto Camera::init() -> void
{
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    center = look_from;

    auto theta = glm::radians(fovy);
    auto h = glm::tan(theta / 2.0);
    auto viewport_height = 2.0 * h * focus_dist;
    auto viewport_width = viewport_height * static_cast<double>(image_width) / image_height;

    // Calculate basis vectors
    forward = glm::normalize(look_from - look_at);
    right = glm::normalize(glm::cross(world_up, forward));
    up = glm::cross(forward, right);

    auto viewport_u = glm::vec3(viewport_width) * right;
    auto viewport_v = glm::vec3(viewport_height) * -up;

    pixel_delta_v = viewport_v / glm::vec3(image_height);
    pixel_delta_u = viewport_u / glm::vec3(image_width);

    auto viewport_upper_left = center - glm::vec3(focus_dist) * forward - viewport_u * glm::vec3(0.5) - viewport_v * glm::vec3(0.5);
    pixel00_loc = viewport_upper_left + glm::vec3(0.5) * (pixel_delta_u + pixel_delta_v);

    // Calculate camera defocus disk basis vectors
    auto defocus_radius = focus_dist * glm::tan(glm::radians(defocus_angle / 2.0f));
    defocus_disk_u = right * glm::vec3(defocus_radius);
    defocus_disk_v = up * glm::vec3(defocus_radius);

    spp_scale = 1.0 / spp;
}

auto Camera::render(const World& world) -> bool
{
    constexpr auto channels = 3;
    auto size = image_width * image_height * channels;
    std::vector<unsigned char> pixels(size);

    struct Work
    {
        int x;
        int y;
    };

    std::vector<Work> work;
    work.reserve(image_width * image_height);
    for (int y = 0; y < image_height; y++)
    {
        for (int x = 0; x < image_width; x++)
        {
            work.push_back(Work{ x, y });
        }
    }

    std::vector<glm::vec3> colors(image_width * image_height);

    auto work_fn = [&](Work& w)
    {
        ZoneScopedN("Trace per pixel");
        int x = w.x;
        int y = w.y;
        glm::vec3 color = glm::vec3(0.0);
        for (int sample = 0; sample < spp; sample++)
        {
            Ray r = get_ray(x, y);
            color += ray_color(r, max_depth, world);
        }

        int idx = (y * image_width + x);
        colors[idx] = color * spp_scale;
    };

    const auto start = std::chrono::steady_clock::now();

    std::for_each(
        std::execution::par,
        work.begin(),
        work.end(),
        work_fn
    );

    const auto end = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Elapsed: " << elapsed.count() << "\n";

    write_color(pixels, colors);

    int success = stbi_write_png("trace.png", image_width, image_height, channels, pixels.data(), image_width * channels);
    return success ? true : false;
}

auto Camera::ray_color(Ray r, int depth, const World& world) -> glm::vec3
{
    if (depth <= 0)
    {
        return glm::vec3(0.0);
    }

    HitRecord record;
    auto interval = Interval::universe();
    // Removes shadow acne
    interval.min = 0.001;

    if (world.hit(r, interval, record))
    {
        Ray scattered{ .origin = record.p };
        glm::vec3 attenuation{};

        switch (record.mat.type)
        {
        case MaterialType::Lambertian:
        {
            // set radius to 1.0 so the direction is a unit vector
            glm::vec3 direction = record.normal + glm::normalize(random_vector());

            // catch degenerate scatter direction
            if (near_zero(direction))
            {
                direction = record.normal;
            }

            attenuation = record.mat.albedo;
            scattered.direction = direction;
            break;
        }
        case MaterialType::Metal:
        {
            attenuation = record.mat.albedo;
            scattered.direction = glm::reflect(r.direction, record.normal);
            break;
        }
        case MaterialType::Dielectric:
        {
            attenuation = glm::vec3(1.0);

            auto ri = record.front_face ? 1.0f / record.mat.refraction_index : record.mat.refraction_index;
            auto unit_dir = glm::normalize(r.direction);

            auto cos_theta = glm::min(glm::dot(-unit_dir, record.normal), 1.0f);
            auto sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);
            auto cannot_refract = ri * sin_theta > 1.0f;

            glm::vec3 direction;
            if (cannot_refract || reflectance(cos_theta, ri) > random_float())
            {
                direction = glm::reflect(unit_dir, record.normal);
            }
            else
            {
                direction = glm::refract(unit_dir, record.normal, ri);
            }

            scattered.direction = direction;
            break;
        }
        default:
            assert(0 && "Material does not exist");
        }

        return attenuation * ray_color(scattered, depth - 1, world);
    }

    glm::vec3 unit_direction = glm::normalize(r.direction);
    auto a = glm::vec3(0.5) * glm::vec3(unit_direction.y + 1.0);
    return (glm::vec3(1) - a) * glm::vec3(1) + a * glm::vec3(0.5, 0.7, 1.0);
}

auto Camera::write_color(std::vector<unsigned char>& data, std::vector<glm::vec3>& colors) -> void
{
    for (int i = 0; i < colors.size(); i++)
    {
        auto c = linear_to_gamma(colors[i]);
        auto r = static_cast<int>(255.999 * c.x);
        auto g = static_cast<int>(255.999 * c.y);
        auto b = static_cast<int>(255.999 * c.z);

        auto idx = i * 3;
        data[idx + 0] = static_cast<unsigned char>(r);
        data[idx + 1] = static_cast<unsigned char>(g);
        data[idx + 2] = static_cast<unsigned char>(b);
    }
}

auto Camera::get_ray(int x, int y) const -> Ray
{
    // Samples a unit square [-0.5, 0.5] in x and y
    auto offset = glm::vec3(random_float(-0.5, 0.5), random_float(-0.5, 0.5), 0.0);

    auto pixel_sample = pixel00_loc
        + (glm::vec3(x) + offset.x) * pixel_delta_u
        + (glm::vec3(y) + offset.y) * pixel_delta_v;

    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_dir = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_dir);
}

auto Camera::defocus_disk_sample() const -> glm::vec3
{
    // random point on camera defocus disk
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}
