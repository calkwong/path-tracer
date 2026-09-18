#include "rando.h"

#include <glm/geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#

#include <random>

auto random_float() -> float
{
    thread_local std::uniform_real_distribution<float> distribution; // [0.0, 1.0)
    thread_local std::mt19937 generator;
    return distribution(generator);
}

auto random_float(float min, float max) -> float
{
    return min + (max - min) * random_float();
}

auto random_vector() -> glm::vec3
{
    return glm::vec3(random_float(), random_float(), random_float());
}

auto random_vector(float min, float max) -> glm::vec3
{
    return glm::vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}

auto random_in_unit_disk() -> glm::vec3
{
    while (true)
    {
        auto p = glm::vec3(random_float(-1, 1), random_float(-1, 1), 0);
        if (glm::dot(p, p) < 1)
        {
            return p;
        }
    }
}
