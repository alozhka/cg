#pragma once
#include <cmath>

struct Vec3
{
	float x = 0, y = 0, z = 0;

	float Dot(const Vec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3 Cross(const Vec3& other) const
	{
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}

	float GetLength() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	Vec3 Normalized() const
	{
		float length = GetLength();
		return { x / length, y / length, z / length };
	}

	Vec3 operator-(const Vec3& other) const
	{
		return { x - other.x, y - other.y, z - other.z };
	}
};