#pragma once

#include "AABB.hpp"
#include "Ray.hpp"

#include <cmath>
#include <cstdint>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <utility>
#include <vector>

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

class RayMesh
{
public:
	RayMesh() = default;

	RayMesh(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> uvs,
		std::vector<MeshTriangle> triangles)
		: m_positions(std::move(positions))
		, m_normals(std::move(normals))
		, m_uvs(std::move(uvs))
		, m_triangles(std::move(triangles))
	{
		RecomputeBounds();
	}

	const std::vector<glm::vec3>& Positions() const { return m_positions; }
	const std::vector<glm::vec3>& Normals() const { return m_normals; }
	const std::vector<glm::vec2>& UVs() const { return m_uvs; }
	const std::vector<MeshTriangle>& Triangles() const { return m_triangles; }
	const AABB& Bounds() const { return m_bounds; }

	bool HasNormals() const { return !m_normals.empty(); }
	bool HasUVs() const { return !m_uvs.empty(); }

	// Линейный перебор треугольников в локальной системе координат.
	// tMax — верхняя граница (обычно текущий hit.t после трансформации).
	// При успехе заполняет out и возвращает true; не трогает out иначе.
	bool Intersect(const Ray& localRay, float tMax, MeshHit& out) const
	{
		if (!m_bounds.Intersect(localRay, tMax))
		{
			return false;
		}

		bool any = false;
		float closest = tMax;
		for (std::uint32_t i = 0; i < m_triangles.size(); ++i)
		{
			const MeshTriangle& tri = m_triangles[i];
			TriangleHit th;
			if (IntersectTriangle(
				localRay,
				m_positions[tri.i0], m_positions[tri.i1], m_positions[tri.i2],
				closest, th))
			{
				closest = th.t;
				out.t = th.t;
				out.u = th.u;
				out.v = th.v;
				out.triangleIndex = i;
				any = true;
			}
		}
		return any;
	}

	// Геометрическая (плоская) нормаль грани в локальной системе.
	glm::vec3 FaceNormal(std::uint32_t triangleIndex) const
	{
		const MeshTriangle& tri = m_triangles[triangleIndex];
		const glm::vec3 e1 = m_positions[tri.i1] - m_positions[tri.i0];
		const glm::vec3 e2 = m_positions[tri.i2] - m_positions[tri.i0];
		return glm::normalize(glm::cross(e1, e2));
	}

	// Интерполяция вершинной нормали по барицентрикам (u, v) Möller-Trumbore.
	// Возвращает нормированный результат. Если вершинных нормалей нет — плоская.
	glm::vec3 InterpolatedNormal(std::uint32_t triangleIndex, float u, float v) const
	{
		if (!HasNormals())
		{
			return FaceNormal(triangleIndex);
		}
		const MeshTriangle& tri = m_triangles[triangleIndex];
		const float w = 1.f - u - v;
		return glm::normalize(
			m_normals[tri.i0] * w +
			m_normals[tri.i1] * u +
			m_normals[tri.i2] * v);
	}

	glm::vec2 InterpolatedUV(std::uint32_t triangleIndex, float u, float v) const
	{
		if (!HasUVs())
		{
			return glm::vec2{ 0.f };
		}
		const MeshTriangle& tri = m_triangles[triangleIndex];
		const float w = 1.f - u - v;
		return m_uvs[tri.i0] * w + m_uvs[tri.i1] * u + m_uvs[tri.i2] * v;
	}

private:
	struct TriangleHit
	{
		float t;
		float u;
		float v;
	};

	// Möller-Trumbore без backface culling — видим обе стороны.
	static bool IntersectTriangle(
		const Ray& ray,
		const glm::vec3& v0,
		const glm::vec3& v1,
		const glm::vec3& v2,
		float tMax,
		TriangleHit& out)
	{
		constexpr float EPS = 1e-7f;
		const glm::vec3 e1 = v1 - v0;
		const glm::vec3 e2 = v2 - v0;
		const glm::vec3 pvec = glm::cross(ray.direction, e2);
		const float det = glm::dot(e1, pvec);
		if (std::abs(det) < EPS)
		{
			return false;
		}
		const float invDet = 1.f / det;
		const glm::vec3 tvec = ray.origin - v0;
		const float u = glm::dot(tvec, pvec) * invDet;
		if (u < 0.f || u > 1.f)
		{
			return false;
		}
		const glm::vec3 qvec = glm::cross(tvec, e1);
		const float v = glm::dot(ray.direction, qvec) * invDet;
		if (v < 0.f || u + v > 1.f)
		{
			return false;
		}
		const float t = glm::dot(e2, qvec) * invDet;
		if (t < 1e-4f || t >= tMax)
		{
			return false;
		}
		out.t = t;
		out.u = u;
		out.v = v;
		return true;
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
	std::vector<glm::vec2> m_uvs;
	std::vector<MeshTriangle> m_triangles;
	AABB m_bounds;
};