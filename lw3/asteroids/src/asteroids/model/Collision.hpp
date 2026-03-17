#pragma once
#include "../../graphics/Vec2f.hpp"

#include <algorithm>
#include <vector>

class Collision
{
public:
	static bool PointInPolygon(Vec2f point, Vec2f center, const std::vector<Vec2f>& polygon)
	{
		int count = polygon.size();
		if (count < 3)
		{
			return false;
		}

		for (int i = 0; i < count; ++i)
		{
			int next = (i + 1) % count;
			if (PointInTriangle(point, center, polygon[i], polygon[next]))
			{
				return true;
			}
		}

		return false;
	}

	static bool PolygonsOverlap(
		const std::vector<Vec2f>& verticesA,
		Vec2f centerB, const std::vector<Vec2f>& verticesB)
	{
		return std::ranges::any_of(verticesA, [&](const Vec2f& v) {
			return PointInPolygon(v, centerB, verticesB);
		});
	}

private:
	static bool PointInTriangle(Vec2f p, Vec2f a, Vec2f b, Vec2f c)
	{
		float z1 = CrossZ(p, a, b);
		float z2 = CrossZ(p, b, c);
		float z3 = CrossZ(p, c, a);
		bool hasNegative = z1 < 0 || z2 < 0 || z3 < 0;
		bool hasPositive = z1 > 0 || z2 > 0 || z3 > 0;
		return !(hasNegative && hasPositive);
	}

	static float CrossZ(Vec2f point, Vec2f v1, Vec2f v2)
	{
		Vec2f triangleEdge = v2 - v1;
		Vec2f edgeToPoint = point - v1;
		return triangleEdge.CrossZ(edgeToPoint);
	}
};