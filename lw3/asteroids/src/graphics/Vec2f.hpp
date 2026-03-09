#pragma once

#include <cmath>

struct Vec2f
{
	float x = 0, y = 0;

	Vec2f operator+(const Vec2f& other) const { return { x + other.x, y + other.y }; }
	Vec2f operator-(const Vec2f& other) const { return { x - other.x, y - other.y }; }
	Vec2f operator*(float s) const { return { x * s, y * s }; }
	Vec2f operator-() const { return { -x, -y }; }

	Vec2f& operator+=(const Vec2f& other) { x += other.x; y += other.y; return *this; }
	Vec2f& operator-=(const Vec2f& other) { x -= other.x; y -= other.y; return *this; }
	Vec2f& operator*=(float s) { x *= s; y *= s; return *this; }

	float Length() const { return std::sqrt(x * x + y * y); }
	float LengthSquared() const { return x * x + y * y; }

	Vec2f Normalized() const
	{
		float len = Length();
		if (len < 1e-8f) return { 0, 0 };
		return { x / len, y / len };
	}

	static float Dot(const Vec2f& a, const Vec2f& b) { return a.x * b.x + a.y * b.y; }
	static float Cross(const Vec2f& a, const Vec2f& b) { return a.x * b.y - a.y * b.x; }
};