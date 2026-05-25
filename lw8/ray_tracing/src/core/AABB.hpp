#pragma once

#include "../core/Ray.hpp"

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
		tNear = tMin;
		tFar = tMax;
		for (int k = 0; k < 3; ++k)
		{
			const float invD = 1.f / ray.direction[k];
			float t0 = (m_min[k] - ray.origin[k]) * invD;
			float t1 = (m_max[k] - ray.origin[k]) * invD;
			if (invD < 0.f)
			{
				std::swap(t0, t1);
			}
			tNear = std::max(tNear, t0);
			tFar = std::min(tFar, t1);
			if (tFar < tNear)
			{
				return false;
			}
		}
		return true;
	}

	bool Intersect(const Ray& ray, float tMax) const
	{
		float tNear = 0.f;
		float tFar = 0.f;
		return Intersect(ray, 0.f, tMax, tNear, tFar);
	}

private:
	glm::vec3 m_min{ std::numeric_limits<float>::infinity() };
	glm::vec3 m_max{ -std::numeric_limits<float>::infinity() };
};