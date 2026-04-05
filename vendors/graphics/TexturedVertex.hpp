#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct TexturedVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};