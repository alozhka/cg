#pragma once

#include "ISceneObject.hpp"

#include <cmath>
#include <glm/geometric.hpp>

class Sphere : public ISceneObject
{
public:
	Sphere(glm::vec3 center, float radius, glm::vec3 color)
		: m_center(center)
		, m_radius(radius)
		, m_color(color)
	{
	}

	bool Intersect(const Ray& ray, HitInfo& hit) const override
	{
		// |o + t*d - c|^2 = r^2; квадратное уравнение с упрощённым b.
		const glm::vec3 oc = ray.origin - m_center;
		const float b = glm::dot(oc, ray.direction);
		const float c = glm::dot(oc, oc) - m_radius * m_radius;
		const float disc = b * b - c;
		if (disc < 0.f)
		{
			return false;
		}

		const float sq = std::sqrt(disc);
		float t = -b - sq;
		if (t < EPSILON)
		{
			t = -b + sq;
		}
		if (t < EPSILON || t >= hit.t)
		{
			return false;
		}

		hit.t = t;
		hit.point = ray.origin + ray.direction * t;
		hit.normal = (hit.point - m_center) / m_radius;
		hit.baseColor = m_color;
		return true;
	}

private:
	static constexpr float EPSILON = 1e-4f;

	glm::vec3 m_center;
	float m_radius;
	glm::vec3 m_color;
};