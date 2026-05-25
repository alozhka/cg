#pragma once

#include "AABB.hpp"
#include "Ray.hpp"

#include <cmath>
#include <cstdint>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <utility>
#include <vector>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

struct MeshTriangle
{
	std::uint32_t i0;
	std::uint32_t i1;
	std::uint32_t i2;
	int materialIndex = -1;
};

struct MeshHit
{
	float t;
	float u;
	float v;
	std::uint32_t triangleIndex;
};

struct RayTriangleHit
{
	float t;
	float u;
	float v;
};

class RayMesh
{
public:
	RayMesh() = default;

	RayMesh(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<MeshTriangle> triangles)
		: m_positions(std::move(positions))
		, m_normals(std::move(normals))
		, m_triangles(std::move(triangles))
	{
		RecomputeBounds();
	}

	const std::vector<glm::vec3>& Positions() const { return m_positions; }
	const std::vector<glm::vec3>& Normals() const { return m_normals; }
	const std::vector<MeshTriangle>& Triangles() const { return m_triangles; }
	const AABB& Bounds() const { return m_bounds; }

	bool Intersect(const Ray& localRay, float tMax, MeshHit& out) const
	{
		if (!m_bounds.Intersect(localRay, tMax))
		{
			return false;
		}

		bool found = false;
		for (std::uint32_t i = 0; i < m_triangles.size(); ++i)
		{
			const MeshTriangle& tri = m_triangles[i];
			std::optional<RayTriangleHit> th = IntersectRayTriangle(
				localRay,
				m_positions[tri.i0], m_positions[tri.i1], m_positions[tri.i2],
				tMax);
			if (th)
			{
				tMax = th->t; // сужаем поиск к ближайшему
				out = MeshHit{ th->t, th->u, th->v, i };
				found = true;
			}
		}
		return found;
	}

	// Интерполяция вершинной нормали по барицентрикам (u, v) Möller-Trumbore.
	// Возвращает нормированный результат.
	glm::vec3 InterpolatedNormal(std::uint32_t triangleIndex, float u, float v) const
	{
		const MeshTriangle& tri = m_triangles[triangleIndex];
		const float w = 1.f - u - v;
		return glm::normalize(
			m_normals[tri.i0] * w +
			m_normals[tri.i1] * u +
			m_normals[tri.i2] * v);
	}

private:
	static std::optional<RayTriangleHit> IntersectRayTriangle(
	const Ray& ray,
	const glm::vec3& v0,
	const glm::vec3& v1,
	const glm::vec3& v2,
	float maxT,
	float minT = 1e-4f)
	{
		constexpr float PARALLEL_EPS = 1e-7f;

		const glm::vec3 e1 = v1 - v0;
		const glm::vec3 e2 = v2 - v0;
		const glm::vec3 pvec = glm::cross(ray.direction, e2);
		const float det = glm::dot(e1, pvec);
		if (std::abs(det) < PARALLEL_EPS)
		{
			return std::nullopt;
		}

		const float invDet = 1.f / det;
		const glm::vec3 tvec = ray.origin - v0;
		const float u = glm::dot(tvec, pvec) * invDet;
		if (u < 0.f || u > 1.f)
		{
			return std::nullopt;
		}

		const glm::vec3 qvec = glm::cross(tvec, e1);
		const float v = glm::dot(ray.direction, qvec) * invDet;
		if (v < 0.f || u + v > 1.f)
		{
			return std::nullopt;
		}

		const float t = glm::dot(e2, qvec) * invDet;
		if (t < minT || t >= maxT)
		{
			return std::nullopt;
		}

		return RayTriangleHit{ t, u, v };
	}

	void RecomputeBounds()
	{
		m_bounds = AABB{};
		for (const auto& p : m_positions)
		{
			m_bounds.Grow(p);
		}
	}

	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec3> m_normals;
	std::vector<MeshTriangle> m_triangles;
	AABB m_bounds;
};