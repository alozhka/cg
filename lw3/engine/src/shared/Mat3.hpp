#pragma once

#include <array>
#include <cmath>
#include <numbers>

// Column-major 3x3 matrix for 2D transforms (compatible with GLSL mat3)
struct Mat3
{
	std::array<float, 9> m{};
	// m[0]=m00, m[1]=m10, m[2]=m20
	// m[3]=m01, m[4]=m11, m[5]=m21
	// m[6]=m02, m[7]=m12, m[8]=m22

	Mat3() = default;

	static Mat3 Identity()
	{
		Mat3 r;
		r.m[0] = 1; r.m[1] = 0; r.m[2] = 0;
		r.m[3] = 0; r.m[4] = 1; r.m[5] = 0;
		r.m[6] = 0; r.m[7] = 0; r.m[8] = 1;
		return r;
	}

	static Mat3 Translation(float x, float y)
	{
		Mat3 r = Identity();
		r.m[6] = x;
		r.m[7] = y;
		return r;
	}

	static Mat3 RotationDegrees(float angleDeg)
	{
		float rad = angleDeg * static_cast<float>(std::numbers::pi / 180.0);
		float c = std::cos(rad);
		float s = std::sin(rad);
		Mat3 r = Identity();
		r.m[0] = c;  r.m[3] = -s;
		r.m[1] = s;  r.m[4] = c;
		return r;
	}

	static Mat3 Scale(float sx, float sy)
	{
		Mat3 r = Identity();
		r.m[0] = sx;
		r.m[4] = sy;
		return r;
	}

	Mat3 operator*(const Mat3& b) const
	{
		Mat3 r;
		for (int col = 0; col < 3; ++col)
			for (int row = 0; row < 3; ++row)
				r.m[col * 3 + row] =
					m[row + 0] * b.m[col * 3 + 0] +
					m[row + 3] * b.m[col * 3 + 1] +
					m[row + 6] * b.m[col * 3 + 2];
		return r;
	}

	const float* data() const { return m.data(); }
};
