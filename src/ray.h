#pragma once

#include <glm/ext/vector_float3.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    auto at(float t) const -> glm::vec3;
};
