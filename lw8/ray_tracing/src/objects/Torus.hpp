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

// Тор с осью вдоль локального Y.
// F(p) = (p·p + R² − r²)² − 4R²(pₓ² + p_z²)
//
// Точка пересечения ищется brute-force сэмплингом F вдоль луча внутри
// ограничивающей коробки: смена знака между соседними сэмплами или |F| ниже
// абсолютного порога считается срабатыванием, далее простая бисекция.
class Torus : public ISceneObject
{
public:
	Torus(float majorRadius, float minorRadius,
		const glm::mat4& transform, MaterialPtr material)
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
		const Ray localRay = CreateLocalRay(ray);

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
	static constexpr int SEARCH_STEPS = 2048;
	static constexpr int REFINE_ITERATIONS = 24;
	static constexpr float MIN_RAY_DISTANCE = 1e-4f;
	static constexpr float NEAR_ZERO_THRESHOLD = 1e-3f;

	struct Sample
	{
		float t;
		float value;
	};

	Ray CreateLocalRay(const Ray& worldRay) const
	{
		const glm::vec3 origin = glm::vec3(m_invTransform * glm::vec4(worldRay.origin, 1));
		const glm::vec3 direction = glm::mat3(m_invTransform) * worldRay.direction;
		return { origin, direction };
	}

	bool FindSurfaceHit(
		const Ray& ray,
		float startT, float endT,
		float maxT,
		float& outT) const
	{
		const float step = (endT - startT) / static_cast<float>(SEARCH_STEPS);
		Sample previous{ startT, Evaluate(ray.At(startT)) };

		for (int i = 1; i <= SEARCH_STEPS; ++i)
		{
			const float t = (i == SEARCH_STEPS) ? endT : startT + step * static_cast<float>(i);
			const Sample current{ t, Evaluate(ray.At(t)) };

			if (HasRootBetween(previous, current))
			{
				const float candidate = RefineRoot(ray, previous.t, current.t);
				if (candidate >= MIN_RAY_DISTANCE && candidate <= maxT)
				{
					outT = candidate;
					return true;
				}
			}

			previous = current;
		}
		return false;
	}

	static bool HasRootBetween(const Sample& a, const Sample& b)
	{
		const bool signFlip = a.value * b.value < 0.f;
		const bool nearZero = std::abs(a.value) < NEAR_ZERO_THRESHOLD
			|| std::abs(b.value) < NEAR_ZERO_THRESHOLD;
		return signFlip || nearZero;
	}

	float RefineRoot(const Ray& ray, float left, float right) const
	{
		float leftValue = Evaluate(ray.At(left));
		for (int i = 0; i < REFINE_ITERATIONS; ++i)
		{
			const float mid = 0.5f * (left + right);
			const float midValue = Evaluate(ray.At(mid));
			if (leftValue * midValue <= 0.f)
			{
				right = mid;
			}
			else
			{
				left = mid;
				leftValue = midValue;
			}
		}
		return 0.5f * (left + right);
	}

	void FillHit(const Ray& ray, float t, HitInfo& hit) const
	{
		const glm::vec3 localPoint = ray.At(t);
		hit.t = t;
		hit.point = glm::vec3(m_transform * glm::vec4(localPoint, 1.f));
		hit.normal = glm::normalize(m_normalMatrix * SurfaceNormal(localPoint));
		hit.material = m_material;
	}

	float Evaluate(const glm::vec3& p) const
	{
		const float sum = glm::dot(p, p) + m_R * m_R - m_r * m_r;
		return sum * sum - 4.f * m_R * m_R * (p.x * p.x + p.z * p.z);
	}

	// Геометрическая нормаль: ближайшая точка центральной окружности тубы
	// лежит в плоскости XZ на расстоянии R от оси Y. Нормаль — единичный
	// вектор от этой точки к точке поверхности.
	glm::vec3 SurfaceNormal(const glm::vec3& p) const
	{
		const float lenXZ = std::sqrt(p.x * p.x + p.z * p.z);
		if (lenXZ < 1e-6f)
		{
			return glm::vec3{ 0.f, p.y >= 0.f ? 1.f : -1.f, 0.f };
		}
		const float k = m_R / lenXZ;
		const glm::vec3 ringCenter{ p.x * k, 0.f, p.z * k };
		return glm::normalize(p - ringCenter);
	}

	float m_R;
	float m_r;
	glm::mat4 m_transform;
	glm::mat4 m_invTransform;
	glm::mat3 m_normalMatrix;
	MaterialPtr m_material;
	AABB m_bounds;
};
