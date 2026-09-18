#pragma once

#include "interval.h"

#include <glm/ext/vector_float3.hpp>

struct Ray;

enum class MaterialType
{
    Lambertian = 0,
    Metal = 1,
    Dielectric,
};

struct Material
{
    MaterialType type;
    glm::vec3 albedo;
    float refraction_index = 1.5; // TODO: what default?
};

struct HitRecord
{
    glm::vec3 p;
    glm::vec3 normal;
    float t;
    bool front_face;
    Material mat;

    // outward_normal assumed to be unit vector
    auto set_face_normal(const Ray& r, const glm::vec3& outward_normal) -> void;
};

struct Sphere
{
    glm::vec3 center;
    float radius;
    Material mat;

    auto hit(const Ray& r, Interval interval, HitRecord& rec) const -> bool;
};
