#pragma once

#include "ray.h"

#include <glm/ext/vector_float3.hpp>

#include <vector>

struct World;

class Camera
{
public:
    float aspect_ratio = 1.0;
    int image_width = 128;
    int spp = 10;
    int max_depth = 10;
    float fovy = 90.f;

    glm::vec3 look_from = glm::vec3(0);
    glm::vec3 look_at = glm::vec3(0, 0, -1);
    glm::vec3 world_up = glm::vec3(0, 1, 0);

    float defocus_angle = 0.f; // Variation angle of rays through each pixel
    float focus_dist = 10.f; // Distance from camera look_from to plane of perfect focus

    auto init() -> void;
    auto render(const World& world) -> bool;

private:
    int image_height;
    glm::vec3 center;
    glm::vec3 pixel00_loc;
    glm::vec3 pixel_delta_u;
    glm::vec3 pixel_delta_v;
    float spp_scale;

    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;
    glm::vec3 defocus_disk_u; // defocus disk horizontal radius
    glm::vec3 defocus_disk_v; // defocus disk vertical radius

    auto ray_color(Ray r, int depth, const World& world) -> glm::vec3;
    auto write_color(std::vector<unsigned char>& data, std::vector<glm::vec3>& colors) -> void;

    // Construct a camera ray originating from defocus disk and directed at a randomly
    // sampled point around pixel location x, y
    auto get_ray(int x, int y) const -> Ray;

    auto defocus_disk_sample() const -> glm::vec3;
};
