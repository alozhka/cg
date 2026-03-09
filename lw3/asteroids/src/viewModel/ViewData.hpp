#pragma once

#include "../graphics/Vec2f.hpp"
#include <vector>

struct ShipViewModel
{
	Vec2f position;
	float angle = 0;
	bool alive = true;
	bool thrusting = false;
	std::vector<Vec2f> vertices;
	std::vector<Vec2f> flameVertices;
};

struct AsteroidViewModel
{
	Vec2f position;
	float angle = 0;
	std::vector<Vec2f> vertices;
};

struct BulletViewModel
{
	Vec2f position;
};

struct DebrisViewModel
{
	Vec2f position;
	float angle = 0;
	Vec2f lineStart;
	Vec2f lineEnd;
	float alpha = 1.0f;
};
