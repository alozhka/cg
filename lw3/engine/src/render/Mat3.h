#pragma once
#include <array>

struct Mat3
{
	std::array<std::array<double, 3>, 3> m{};

	static Mat3 Identity()
	{
		Mat3 r;
		r.m = { {
			{ 1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, 0, 1 }
		} };
		return r;
	}

	static Mat3 Ortho(double left, double right, double bottom, double top)
	{
		Mat3 r;
		r.m[0][0] = 2.0 / (right - left);
		r.m[1][1] = 2.0 / (top - bottom);
		r.m[2][0] = -(right + left) / (right - left);
		r.m[2][1] = -(top + bottom) / (top - bottom);
		r.m[2][2] = 1;
		return r;
	}
};