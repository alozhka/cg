#pragma once

#include <graphics/camera/FirstPersonCamera.hpp>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

struct Ray
{
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		: origin(origin)
		, direction(glm::normalize(direction))
	{
	}

	glm::vec3 origin{ 0 };
	glm::vec3 direction{ 0, 0, 1 };
};