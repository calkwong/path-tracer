#include "ray.h"

#include <glm/ext/vector_float3.hpp>

auto Ray::at(float t) const -> glm::vec3
{
    return origin + t * direction;
}
