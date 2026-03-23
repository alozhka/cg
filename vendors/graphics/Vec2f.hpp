#pragma once
#include <cmath>

struct Vec2f
{
	float x = 0, y = 0;

	float GetLength() const
	{
		return std::sqrt(x * x + y * y);
	}

	Vec2f GetNormalized() const
	{
		float length = GetLength();
		return { x / length, y / length };
	}

	Vec2f operator*(float n) const
	{
		return { x * n, y * n };
	}

	Vec2f operator+(const Vec2f& other) const
	{
		return { x + other.x, y + other.y };
	}

	Vec2f operator-(const Vec2f& other) const
	{
		return { x - other.x, y - other.y };
	}

	Vec2f operator*=(float n)
	{
		x *= n;
		y *= n;
		return *this;
	}

	Vec2f& operator+=(const Vec2f& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	float CrossZ(const Vec2f& other) const
	{
		return x * other.y - y * other.x;
	}
};