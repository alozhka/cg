#pragma once

#include <glm/vec3.hpp>
#include <limits>

struct HitInfo
{
	float t = std::numeric_limits<float>::infinity();
	glm::vec3 point{ 0.f };
	glm::vec3 normal{ 0.f };
	// Базовый диффузный цвет точки. На этапе 3 заменится на индекс/ссылку
	// на материал.
	glm::vec3 baseColor{ 1.f };
};