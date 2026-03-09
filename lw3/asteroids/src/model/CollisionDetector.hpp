#pragma once

#include "../graphics/Vec2f.hpp"
#include <cmath>
#include <numbers>
#include <vector>

namespace CollisionDetector
{

inline Vec2f TransformPoint(const Vec2f& p, const Vec2f& pos, float angleDeg, float scale = 1.0f)
{
	float rad = angleDeg * std::numbers::pi_v<float> / 180.0f;
	float c = std::cos(rad);
	float s = std::sin(rad);
	return {
		pos.x + (p.x * c - p.y * s) * scale,
		pos.y + (p.x * s + p.y * c) * scale
	};
}

inline std::vector<Vec2f> TransformPolygon(
	const std::vector<Vec2f>& vertices,
	const Vec2f& pos,
	float angleDeg,
	float scale = 1.0f)
{
	std::vector<Vec2f> result;
	result.reserve(vertices.size());
	for (const auto& v : vertices)
	{
		result.push_back(TransformPoint(v, pos, angleDeg, scale));
	}
	return result;
}

inline bool PointInTriangle(const Vec2f& p, const Vec2f& a, const Vec2f& b, const Vec2f& c)
{
	Vec2f v0 = c - a;
	Vec2f v1 = b - a;
	Vec2f v2 = p - a;

	float dot00 = Vec2f::Dot(v0, v0);
	float dot01 = Vec2f::Dot(v0, v1);
	float dot02 = Vec2f::Dot(v0, v2);
	float dot11 = Vec2f::Dot(v1, v1);
	float dot12 = Vec2f::Dot(v1, v2);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v <= 1);
}

inline bool PointInPolygon(const Vec2f& p, const std::vector<Vec2f>& polygon)
{
	if (polygon.size() < 3) return false;

	Vec2f centroid{ 0, 0 };
	for (const auto& v : polygon)
	{
		centroid += v;
	}
	centroid *= (1.0f / static_cast<float>(polygon.size()));

	size_t n = polygon.size();
	for (size_t i = 0; i < n; i++)
	{
		const Vec2f& a = centroid;
		const Vec2f& b = polygon[i];
		const Vec2f& c = polygon[(i + 1) % n];
		if (PointInTriangle(p, a, b, c))
		{
			return true;
		}
	}
	return false;
}

inline bool PolygonsIntersect(const std::vector<Vec2f>& polyA, const std::vector<Vec2f>& polyB)
{
	for (const auto& v : polyA)
	{
		if (PointInPolygon(v, polyB)) return true;
	}
	for (const auto& v : polyB)
	{
		if (PointInPolygon(v, polyA)) return true;
	}
	return false;
}

} // namespace CollisionDetector
