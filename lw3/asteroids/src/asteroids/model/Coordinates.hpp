#pragma once
#include "../../graphics/Vec2f.hpp"

class Coordinates
{
public:
	static Vec2f Wrap(Vec2f pos, float width, float height, float margin = 0.0f)
	{
		float halfWidth = width * 0.5f + margin;
		float halfHeight = height * 0.5f + margin;
		float totalWidth = width + 2 * margin;
		float totalHeight = height + 2 * margin;

		if (pos.x < -halfWidth)
		{
			pos.x += totalWidth;
		}
		if (pos.x > halfWidth)
		{
			pos.x -= totalWidth;
		}
		if (pos.y < -halfHeight)
		{
			pos.y += totalHeight;
		}
		if (pos.y > halfHeight)
		{
			pos.y -= totalHeight;
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