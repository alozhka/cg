#pragma once

#include "../core/AABB.hpp"
#include "../core/ISceneObject.hpp"
#include "../shading/MaterialData.hpp"

#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <utility>

class Torus : public ISceneObject
{
public:
	Torus(
		float majorRadius,
		float minorRadius,
		const glm::mat4& transform,
		const MaterialPtr& material)
		: m_R(majorRadius)
		, m_r(minorRadius)
		, m_transform(transform)
		, m_invTransform(glm::inverse(transform))
		, m_normalMatrix(glm::transpose(glm::mat3(m_invTransform)))
		, m_material(std::move(material))
		, m_bounds(
			  glm::vec3{ -(m_R + m_r), -m_r, -(m_R + m_r) },
			  glm::vec3{ m_R + m_r, m_r, m_R + m_r })
	{
	}

	bool Intersect(const Ray& ray, HitInfo& hit) const override
	{
		Ray localRay = CreateLocalRay(ray);

		float entryAt = 0;
		float exitAt = 0;
		if (!m_bounds.Intersect(localRay, MIN_RAY_DISTANCE, hit.t, entryAt, exitAt))
		{
			return false;
		}

		float hitAt = 0;
		if (!FindSurfaceHit(localRay, entryAt, exitAt, hit.t, hitAt))
		{
			return false;
		}

		FillHit(localRay, hitAt, hit);
		return true;
	}

private:
	struct Sample
	{
		float t;
		float value;
	};

	Ray CreateLocalRay(const Ray& worldRay) const
	{
		glm::vec3 origin = glm::vec3(m_invTransform * glm::vec4(worldRay.origin, 1));
		glm::vec3 direction = glm::mat3(m_invTransform) * worldRay.direction;
		return { origin, direction };
	}

	bool FindSurfaceHit(
		const Ray& ray,
		float startAt,
		float endAt,
		float max,
		float& hitAt) const
	{
		float step = (endAt - startAt) / SEARCH_STEPS;
		Sample previous{ startAt, Evaluate(ray.At(startAt)) };

		for (int i = 1; i <= SEARCH_STEPS; ++i)
		{
			float t = startAt + step * static_cast<float>(i);
			Sample current{ t, Evaluate(ray.At(t)) };

			if (HasRootBetween(previous, current))
			{
				float candidate = RefineRoot(ray, previous.t, current.t);
				if (candidate >= MIN_RAY_DISTANCE && candidate <= max)
				{
					hitAt = candidate;
					return true;
				}
			}

			previous = current;
		}
		return false;
	}

	static bool HasRootBetween(const Sample& a, const Sample& b)
	{
		const bool isSignFlipped = a.value * b.value < 0;
		const bool isNearZero = glm::abs(a.value) < NEAR_ZERO_THRESHOLD || glm::abs(b.value) < NEAR_ZERO_THRESHOLD;
		return isSignFlipped || isNearZero;
	}

	float RefineRoot(const Ray& ray, float left, float right) const
	{
		float leftValue = Evaluate(ray.At(left));

		for (int i = 0; i < REFINE_ITERATIONS; ++i)
		{
			float mid = 0.5 * (left + right);
			float midValue = Evaluate(ray.At(mid));
			if (leftValue * midValue <= 0)
			{
				right = mid;
			}
			else
			{
				left = mid;
				leftValue = midValue;
			}
		}

		return 0.5 * (left + right);
	}

	void FillHit(const Ray& ray, float t, HitInfo& hit) const
	{
		const glm::vec3 localPoint = ray.At(t);
		hit.t = t;
		hit.point = glm::vec3(m_transform * glm::vec4(localPoint, 1));
		hit.normal = glm::normalize(m_normalMatrix * SurfaceNormal(localPoint));
		hit.material = m_material;
	}

	// F(p) = (p·p + R² − r²)² − 4R²(p_x² + p_z²)
	float Evaluate(const glm::vec3& p) const
	{
		float sum = glm::dot(p, p) + m_R * m_R - m_r * m_r;
		return sum * sum - 4 * m_R * m_R * (p.x * p.x + p.z * p.z);
	}

	glm::vec3 SurfaceNormal(const glm::vec3& p) const
	{
		glm::vec2 pXZ = { p.x, p.z };
		float length = glm::length(pXZ);

		if (length < 1e-6f)
		{
			return glm::vec3{ 0, p.y >= 0 ? 1 : -1, 0 };
		}

		glm::vec2 circle = pXZ * m_R / length;
		glm::vec3 center{ circle.x, 0, circle.y };

		return glm::normalize(p - center);
	}

	static constexpr int SEARCH_STEPS = 2048;
	static constexpr int REFINE_ITERATIONS = 12;
	static constexpr float MIN_RAY_DISTANCE = 1e-4f;
	static constexpr float NEAR_ZERO_THRESHOLD = 1e-3f;

	float m_R;
	float m_r;
	glm::mat4 m_transform;
	glm::mat4 m_invTransform;
	glm::mat3 m_normalMatrix;
	MaterialPtr m_material;
	AABB m_bounds;
};
