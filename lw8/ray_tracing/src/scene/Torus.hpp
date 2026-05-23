#pragma once

#include "../shading/MaterialData.hpp"
#include "ISceneObject.hpp"

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
// ограничивающей сферы: смена знака между соседними сэмплами или |F| ниже
// абсолютного порога считается срабатыванием, далее простая бисекция.
//
// Аналитика квартики вынесена в бонус — этот путь проще и устойчивее
// к касательным лучам.
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
	{
	}

	bool Intersect(const Ray& ray, HitInfo& hit) const override
	{
		// Луч в локальную систему. Преобразуем origin и (origin + direction)
		// как точки — это даёт корректный локальный direction даже при
		// неединичном масштабе матрицы.
		const glm::vec3 oLocal = glm::vec3(m_invTransform * glm::vec4(ray.origin, 1.f));
		const glm::vec3 eLocal = glm::vec3(m_invTransform * glm::vec4(ray.origin + ray.direction, 1.f));
		const glm::vec3 dLocal = eLocal - oLocal;

		float tStart = 0.f;
		float tEnd = 0.f;
		if (!IntersectBoundingSphere(oLocal, dLocal, EPSILON, hit.t, tStart, tEnd))
		{
			return false;
		}

		const float step = (tEnd - tStart) / static_cast<float>(SEARCH_STEPS);
		float prevT = tStart;
		float prevF = Evaluate(oLocal + dLocal * prevT);

		for (int i = 1; i <= SEARCH_STEPS; ++i)
		{
			const float curT = (i == SEARCH_STEPS) ? tEnd : (tStart + step * static_cast<float>(i));
			const float curF = Evaluate(oLocal + dLocal * curT);

			const bool signFlip = prevF * curF < 0.f;
			const bool nearZero = std::abs(prevF) < TANGENT_EPS || std::abs(curF) < TANGENT_EPS;
			if (signFlip || nearZero)
			{
				const float tHit = RefineRoot(oLocal, dLocal, prevT, curT);
				if (tHit < EPSILON || tHit >= hit.t)
				{
					return false;
				}

				const glm::vec3 pLocal = oLocal + dLocal * tHit;
				glm::vec3 nWorld = glm::normalize(m_normalMatrix * SurfaceNormal(pLocal));
				if (glm::dot(nWorld, ray.direction) > 0.f)
				{
					nWorld = -nWorld;
				}

				hit.t = tHit;
				hit.point = ray.origin + ray.direction * tHit;
				hit.normal = nWorld;
				hit.material = m_material;
				return true;
			}

			prevT = curT;
			prevF = curF;
		}
		return false;
	}

private:
	static constexpr int SEARCH_STEPS = 1024;
	static constexpr int REFINE_ITERATIONS = 12;
	static constexpr float EPSILON = 1e-4f;
	static constexpr float TANGENT_EPS = 1e-4f;

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
			// Точка на оси Y — нормаль направлена строго вверх/вниз.
			return glm::vec3{ 0.f, p.y >= 0.f ? 1.f : -1.f, 0.f };
		}
		const float k = m_R / lenXZ;
		const glm::vec3 ringCenter{ p.x * k, 0.f, p.z * k };
		return glm::normalize(p - ringCenter);
	}

	bool IntersectBoundingSphere(
		const glm::vec3& o, const glm::vec3& d,
		float minT, float maxT,
		float& start, float& end) const
	{
		const float a = glm::dot(d, d);
		const float halfB = glm::dot(o, d);
		const float radius = m_R + m_r;
		const float c = glm::dot(o, o) - radius * radius;
		const float disc = halfB * halfB - a * c;
		if (disc < 0.f)
		{
			return false;
		}
		const float sq = std::sqrt(disc);
		start = std::max(minT, (-halfB - sq) / a);
		end = std::min(maxT, (-halfB + sq) / a);
		return end >= start;
	}

	float RefineRoot(const glm::vec3& o, const glm::vec3& d, float left, float right) const
	{
		float leftValue = Evaluate(o + d * left);
		for (int i = 0; i < REFINE_ITERATIONS; ++i)
		{
			const float mid = 0.5f * (left + right);
			const float midValue = Evaluate(o + d * mid);
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

	float m_R;
	float m_r;
	glm::mat4 m_transform;
	glm::mat4 m_invTransform;
	glm::mat3 m_normalMatrix;
	MaterialPtr m_material;
};