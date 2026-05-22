#pragma once

#include "core/HitInfo.hpp"
#include "core/Ray.hpp"
#include "scene/Scene.hpp"

#include <algorithm>
#include <cstdint>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

// Простейшая Lambert-модель с одним направленным источником света. На этапе 3
// здесь появится Phong + материалы; пока — только diffuse + ambient для
// визуальной проверки геометрии.
inline glm::vec3 TraceRay(const Scene& scene, const Ray& ray)
{
	HitInfo hit;
	if (!scene.Intersect(ray, hit))
	{
		// Простой градиент неба, чтобы было видно где «промах».
		const float t = 0.5f * (ray.direction.y + 1.f);
		return glm::mix(glm::vec3{ 1.f }, glm::vec3{ 0.5f, 0.7f, 1.f }, t);
	}

	const glm::vec3 lightDir = glm::normalize(glm::vec3{ -1.f, -1.f, -0.4f });
	const glm::vec3 ambient{ 0.15f };
	const float ndl = std::max(0.f, glm::dot(hit.normal, -lightDir));

	return hit.baseColor * (ambient + glm::vec3{ ndl });
}

inline std::uint32_t PackColor(glm::vec3 c)
{
	c = glm::clamp(c, glm::vec3{ 0.f }, glm::vec3{ 1.f });
	const auto r = static_cast<std::uint8_t>(c.r * 255.f);
	const auto g = static_cast<std::uint8_t>(c.g * 255.f);
	const auto b = static_cast<std::uint8_t>(c.b * 255.f);
	const std::uint8_t a = 0xff;
	return std::uint32_t(a) << 24 | std::uint32_t(r) << 16 | std::uint32_t(g) << 8 | std::uint32_t(b);
}