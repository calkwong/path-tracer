#pragma once

#include "hittable.h"
#include "interval.h"

#include <vector>

struct Ray;

struct World
{
    std::vector<Sphere> spheres;

    auto add_sphere(Sphere sphere) -> void;
    auto hit(const Ray& r, Interval interval, HitRecord& rec) const -> bool;
};
