#pragma once

class vec3;
class Material;

struct Sphere
{
public:
	vec3 center{};
	float radius{};
	std::unique_ptr<Material> mat;
};