#pragma once
#include "../../graphics/Vec2f.hpp"

class Coordinates
{
public:
	static Vec2f Wrap(Vec2f pos, float width, float height)
	{
		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;

		if (pos.x < -halfWidth)
		{
			pos.x += width;
		}
		if (pos.x > halfWidth)
		{
			pos.x -= width;
		}
		if (pos.y < -halfHeight)
		{
			pos.y += height;
		}
		if (pos.y > halfHeight)
		{
			pos.y -= height;
		}

		return pos;
	}

	static float WrapAngle(float angle)
	{
		if (angle > 360)
		{
			angle -= 360;
		}
		if (angle < 0)
		{
			angle += 360;
		}

		return angle;
	}
};