#include "hittable.h"

#include "ray.h"
#include "interval.h"

#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/ext/vector_float3.hpp>

auto HitRecord::set_face_normal(const Ray& r, const glm::vec3& outward_normal) -> void
{
    front_face = glm::dot(r.direction, outward_normal) < 0.0;
    normal = front_face ? outward_normal : -outward_normal;
}

auto Sphere::hit(const Ray& r, Interval ray_t, HitRecord& rec) const -> bool
{
    glm::vec3 oc = center - r.origin;
    auto a = glm::dot(r.direction, r.direction);
    auto h = glm::dot(r.direction, oc);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto discriminant = h * h - a * c;

    if (discriminant < 0)
    {
        return false;
    }

    auto sqrt_discriminant = glm::sqrt(discriminant);
    auto root = (h - sqrt_discriminant) / a;
    if (!ray_t.surrounds(root))
    {
        root = (h + sqrt_discriminant) / a;
        if (!ray_t.surrounds(root))
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.normal = (rec.p - center) / radius;
    rec.set_face_normal(r, rec.normal);
    rec.mat = mat;

    return true;
}
