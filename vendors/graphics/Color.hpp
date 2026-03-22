#pragma once
#include <array>

struct Color
{
	float r = 0, g = 0, b = 0, a = 1;

	std::array<float, 4> ToFloatArray() const
	{
		return { r, g, b, a };
	}
};