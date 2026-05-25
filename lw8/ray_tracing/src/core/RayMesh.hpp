#pragma once

#include "AABB.hpp"
#include "Ray.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <utility>
#include <vector>

struct MeshTriangle
{
	std::uint32_t i0;
	std::uint32_t i1;
	std::uint32_t i2;
	int materialIndex = -1;
};

struct Barycentric
{
	float u;
	float v;
	float w;

	static Barycentric FromUV(float u, float v)
	{
		return Barycentric{ u, v, 1 - u - v };
	}
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
	Barycentric bary;
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

	const std::vector<glm::vec3>& Positions() const
	{
		return m_positions;
	}

	const std::vector<glm::vec3>& Normals() const
	{
		return m_normals;
	}

	const std::vector<MeshTriangle>& Triangles() const
	{
		return m_triangles;
	}

	const AABB& Bounds() const
	{
		return m_bounds;
	}

	bool Intersect(const Ray& localRay, float tMax, MeshHit& out) const
	{
		if (!m_bounds.Intersect(localRay, tMax))
		{
			return false;
		}

		bool found = false;
		for (std::uint32_t i = 0; i < m_triangles.size(); ++i)
		{
			std::optional<RayTriangleHit> hit = IntersectTriangle(localRay, i, tMax);
			if (!hit)
			{
				continue;
			}

			tMax = hit->t;
			out = MeshHit{ hit->t, hit->bary.u, hit->bary.v, i };
			found = true;
		}
		return found;
	}

	glm::vec3 InterpolatedNormal(std::uint32_t triangleIndex, float u, float v) const
	{
		const std::array<glm::vec3, 3> n = TriangleNormals(m_triangles[triangleIndex]);
		const Barycentric bary = Barycentric::FromUV(u, v);
		return glm::normalize(n[0] * bary.w + n[1] * bary.u + n[2] * bary.v);
	}

private:
	static constexpr float PARALLEL_EPS = 1e-7f;
	static constexpr float MIN_T = 1e-4f;

	std::array<glm::vec3, 3> TriangleNormals(const MeshTriangle& tri) const
	{
		return { m_normals[tri.i0], m_normals[tri.i1], m_normals[tri.i2] };
	}

	std::optional<RayTriangleHit> IntersectTriangle(
		const Ray& ray,
		std::uint32_t triangleIndex,
		float tMax) const
	{
		const std::array<glm::vec3, 3> p = TrianglePositions(m_triangles[triangleIndex]);
		return IntersectRayTriangle(ray, p[0], p[1], p[2], tMax);
	}

	std::array<glm::vec3, 3> TrianglePositions(const MeshTriangle& tri) const
	{
		return { m_positions[tri.i0], m_positions[tri.i1], m_positions[tri.i2] };
	}

	// P = (1−u−v)·V0 + u·V1 + v·V2
	static std::optional<RayTriangleHit> IntersectRayTriangle(
		const Ray& ray,
		const glm::vec3& v0,
		const glm::vec3& v1,
		const glm::vec3& v2,
		float maxT)
	{
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 pvec = glm::cross(ray.direction, edge2); // P = D × E2
		float det = glm::dot(edge1, pvec);
		if (IsParallelToTriangle(det))
		{
			return std::nullopt;
		}

		float invDet = 1 / det;
		glm::vec3 tvec = ray.origin - v0; // T = O − V0
		glm::vec3 qvec = glm::cross(tvec, edge1); // Q = T × E1

		float u = glm::dot(tvec, pvec) * invDet;
		float v = glm::dot(ray.direction, qvec) * invDet;
		if (!IsInsideTriangle(u, v))
		{
			return std::nullopt;
		}

		float t = glm::dot(edge2, qvec) * invDet;
		if (t < MIN_T || t >= maxT)
		{
			return std::nullopt;
		}

		return RayTriangleHit{ t, Barycentric::FromUV(u, v) };
	}

	static bool IsParallelToTriangle(float det)
	{
		return std::abs(det) < PARALLEL_EPS;
	}

	static bool IsInsideTriangle(float u, float v)
	{
		return u >= 0 && v >= 0 && u + v <= 1;
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