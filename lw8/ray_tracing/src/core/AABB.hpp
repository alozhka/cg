#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "../core/Ray.hpp"
#include "glm/gtx/component_wise.hpp"

#include <algorithm>
#include <glm/vec3.hpp>
#include <limits>

class AABB
{
public:
	AABB() = default;

	AABB(const glm::vec3& min, const glm::vec3& max)
		: m_min(min)
		, m_max(max)
	{
	}

	const glm::vec3& Min() const { return m_min; }
	const glm::vec3& Max() const { return m_max; }

	void Grow(const glm::vec3& p)
	{
		m_min = glm::min(m_min, p);
		m_max = glm::max(m_max, p);
	}

	bool Intersect(const Ray& ray, float tMin, float tMax, float& tNear, float& tFar) const
	{
		const glm::vec3 invDirection = 1.0f / ray.direction;
		const glm::vec3 t0 = (m_min - ray.origin) * invDirection;
		const glm::vec3 t1 = (m_max - ray.origin) * invDirection;

		tNear = std::max(tMin, glm::compMax(glm::min(t0, t1)));
		tFar = std::min(tMax, glm::compMin(glm::max(t0, t1)));
		return tNear <= tFar;
	}

	bool Intersect(const Ray& ray, float tMax) const
	{
		float tNear = 0.f;
		float tFar = 0.f;
		return Intersect(ray, 0, tMax, tNear, tFar);
	}

private:
	glm::vec3 m_min{ std::numeric_limits<float>::infinity() };
	glm::vec3 m_max{ -std::numeric_limits<float>::infinity() };
};