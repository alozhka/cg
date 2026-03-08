#pragma once
#include <array>
#include <cmath>
#include <numbers>

struct Mat3
{
	// Row-major формат
	std::array<std::array<float, 3>, 3> m{};

	Mat3 operator*(const Mat3& other) const
	{
		Mat3 r{};

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				for (size_t k = 0; k < 3; k++)
				{
					r.m[i][j] += m[i][k] * other.m[k][j];
				}
			}
		}

		return r;
	}

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

	static Mat3 Translate(float tx, float ty)
	{
		Mat3 r = Identity();
		r.m[0][2] = tx;
		r.m[1][2] = ty;
		return r;
	}

	static Mat3 Rotate(float degrees)
	{
		float radians = degrees * std::numbers::pi_v<float> / 180;
		float cos = std::cos(radians);
		float sin = std::sin(radians);

		Mat3 r{};
		r.m[0][0] = cos;
		r.m[0][1] = -sin;
		r.m[1][0] = sin;
		r.m[1][1] = cos;
		r.m[2][2] = 1;
		return r;
	}

	static Mat3 Scale(float sx, float sy)
	{
		Mat3 r{};
		r.m[0][0] = sx;
		r.m[1][1] = sy;
		r.m[2][2] = 1;
		return r;
	}

	static Mat3 Ortho(float left, float right, float bottom, float top)
	{
		Mat3 r;
		r.m[0][0] = 2 / (right - left);
		r.m[1][1] = 2 / (top - bottom);
		r.m[0][2] = -(right + left) / (right - left);
		r.m[1][2] = -(top + bottom) / (top - bottom);
		r.m[2][2] = 1;
		return r;
	}

	// Column-based формат
	std::array<float, 9> ToFloatArray() const
	{
		return {
			m[0][0],
			m[1][0],
			m[2][0],
			m[0][1],
			m[1][1],
			m[2][1],
			m[0][2],
			m[1][2],
			m[2][2],
		};
	}
};