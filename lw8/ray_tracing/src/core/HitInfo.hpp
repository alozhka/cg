#pragma once

#include "../shading/MaterialData.hpp"

#include <glm/vec3.hpp>
#include <limits>

struct HitInfo
{
	float t = std::numeric_limits<float>::infinity();
	glm::vec3 point{ 0 };
	glm::vec3 normal{ 0 };
	MaterialPtr material;
};