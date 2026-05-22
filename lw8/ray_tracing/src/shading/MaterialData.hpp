#pragma once

#include <glm/vec3.hpp>
#include <memory>

struct MaterialData
{
	glm::vec3 ambient{ 0.1 };
	glm::vec3 diffuse{ 0.8 };
	glm::vec3 specular{ 0 };
	float shininess = 32;
};

using MaterialPtr = std::shared_ptr<const MaterialData>;