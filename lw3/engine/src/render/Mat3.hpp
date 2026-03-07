#pragma once
#include <array>
#include <cmath>
#include <numbers>

struct Mat3
{
	std::array<std::array<double, 3>, 3> m{};

	static Mat3 Identity()
	{
		Mat3 r;
		r.m = {
			{ { 1, 0, 0 },
				{ 0, 1, 0 },
				{ 0, 0, 1 } }
		};
		return r;
	}

	static Mat3 Translate(double tx, double ty)
	{
		Mat3 r = Identity();
		r.m[0][2] = tx;
		r.m[1][2] = ty;
		return r;
	}

	static Mat3 Rotate(double degrees)
	{
		double radians = degrees * std::numbers::pi / 180;
		double cos = std::cos(radians);
		double sin = std::sin(radians);

		Mat3 r{};
		r.m[0][0] = cos;
		r.m[0][1] = -sin;
		r.m[1][0] = sin;
		r.m[1][1] = cos;
		r.m[2][2] = 1;
		return r;
	}

	static Mat3 Scale(double sx, double sy)
	{
		Mat3 r{};
		r.m[0][0] = sx;
		r.m[1][1] = sy;
		r.m[2][2] = 1;
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