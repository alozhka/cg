#pragma once

#include "ISceneObject.hpp"

#include <cmath>
#include <glm/geometric.hpp>

class Plane : public ISceneObject
{
public:
	Plane(glm::vec3 point, glm::vec3 normal, glm::vec3 color)
		: m_point(point)
		, m_normal(glm::normalize(normal))
		, m_color(color)
	{
	}

	bool Intersect(const Ray& ray, HitInfo& hit) const override
	{
		const float denom = glm::dot(m_normal, ray.direction);
		if (std::abs(denom) < EPSILON)
		{
			return false;
		}

		const float t = glm::dot(m_point - ray.origin, m_normal) / denom;
		if (t < EPSILON || t >= hit.t)
		{
			return false;
		}

		hit.t = t;
		hit.point = ray.origin + ray.direction * t;
		// Нормаль развёрнута в полупространство наблюдателя — корректное
		// освещение с обеих сторон плоскости.
		hit.normal = denom < 0.f ? m_normal : -m_normal;
		hit.baseColor = m_color;
		return true;
	}

private:
	static constexpr float EPSILON = 1e-4f;

	glm::vec3 m_point;
	glm::vec3 m_normal;
	glm::vec3 m_color;
};