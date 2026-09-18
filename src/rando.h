#pragma once

#include <glm/ext/vector_float3.hpp>

auto random_float() -> float;
auto random_float(float min, float max) -> float;
auto random_vector() -> glm::vec3;
auto random_in_unit_disk() -> glm::vec3;
