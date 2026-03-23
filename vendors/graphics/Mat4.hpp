#pragma once
#include "Vec3.hpp"

#include <array>
#include <cmath>
#include <numbers>

struct Mat4
{
	// Row-major формат
	std::array<std::array<float, 4>, 4> m{};

	static Mat4 Identity()
	{
		Mat4 result;

		result.m[0][0] = 1;
		result.m[1][1] = 1;
		result.m[2][2] = 1;
		result.m[3][3] = 1;

		return result;
	}

	static Mat4 Perspective(float fov, float aspectRatio, float zNear, float zFar)
	{
		Mat4 result{};
		float fovRadians = fov * std::numbers::pi_v<float> / 180;
		float halfTan = std::tan(fovRadians / 2);

		result.m[0][0] = 1 / (aspectRatio * halfTan);
		result.m[1][1] = 1 / halfTan;
		result.m[2][2] = (zFar + zNear) / (zNear - zFar);
		result.m[2][3] = 2 * zFar * zNear / (zNear - zFar);
		result.m[3][2] = -1;

		return result;
	}

	static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
	{
		Mat4 result{};

		Vec3 f = (center - eye).Normalized();
		Vec3 s = f.Cross(up).Normalized();
		Vec3 u = s.Cross(f);

		result.m[0][0] = s.x;
		result.m[0][1] = s.y;
		result.m[0][2] = s.z;

		result.m[1][0] = u.x;
		result.m[1][1] = u.y;
		result.m[1][2] = u.z;

		result.m[2][0] = -f.x;
		result.m[2][1] = -f.y;
		result.m[2][2] = -f.z;

		result.m[0][3] = -s.Dot(eye);
		result.m[1][3] = -u.Dot(eye);
		result.m[2][3] = f.Dot(eye);
		result.m[3][3] = 1;

		return result;
	}

	Mat4 operator*(const Mat4& other) const
	{
		Mat4 result{};

		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				result.m[row][col] = m[row][0] * other.m[0][col] + m[row][1] * other.m[1][col] + m[row][2] * other.m[2][col] + m[row][3] * other.m[3][col];
			}
		}

		return result;
	}

	std::array<float, 16> ToFloatArray() const
	{
		return {
			m[0][0],
			m[1][0],
			m[2][0],
			m[3][0],
			m[0][1],
			m[1][1],
			m[2][1],
			m[3][1],
			m[0][2],
			m[1][2],
			m[2][2],
			m[3][2],
			m[0][3],
			m[1][3],
			m[2][3],
			m[3][3],
		};
	}
};
