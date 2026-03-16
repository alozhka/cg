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
		float d1 = (b - a).Cross(p - a);
		float d2 = (c - b).Cross(p - b);
		float d3 = (a - c).Cross(p - c);
		bool hasNegative = (d1 < 0) || (d2 < 0) || (d3 < 0);
		bool hasPositive = (d1 > 0) || (d2 > 0) || (d3 > 0);
		return !(hasNegative && hasPositive);
	}
};