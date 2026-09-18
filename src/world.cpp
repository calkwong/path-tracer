#include "world.h"
#include "hittable.h"
#include "ray.h"
#include "interval.h"

#include <vector>

auto World::add_sphere(Sphere sphere) -> void
{
    spheres.push_back(sphere);
}

auto World::hit(const Ray& r, Interval ray_t, HitRecord& rec) const -> bool
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (auto& sphere : spheres)
    {
        if (sphere.hit(r, Interval{ ray_t.min, closest_so_far }, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
