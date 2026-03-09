#pragma once

#include "../graphics/Vec2f.hpp"
#include <cmath>
#include <numbers>
#include <random>
#include <vector>

enum class AsteroidSize
{
	Large,
	Medium,
	Small
};

struct Asteroid
{
	Vec2f position{ 0, 0 };
	Vec2f velocity{ 0, 0 };
	float angle = 0;
	float angularVelocity = 0;
	AsteroidSize size = AsteroidSize::Large;
	std::vector<Vec2f> vertices;

	static float GetRadius(AsteroidSize s)
	{
		switch (s)
		{
		case AsteroidSize::Large: return 40.0f;
		case AsteroidSize::Medium: return 20.0f;
		case AsteroidSize::Small: return 10.0f;
		}
		return 40.0f;
	}

	static int GetScore(AsteroidSize s)
	{
		switch (s)
		{
		case AsteroidSize::Large: return 20;
		case AsteroidSize::Medium: return 50;
		case AsteroidSize::Small: return 100;
		}
		return 0;
	}

	static int GetVertexCount(AsteroidSize s)
	{
		switch (s)
		{
		case AsteroidSize::Large: return 12;
		case AsteroidSize::Medium: return 9;
		case AsteroidSize::Small: return 7;
		}
		return 10;
	}

	static std::vector<Vec2f> GenerateVertices(AsteroidSize s, std::mt19937& rng)
	{
		float radius = GetRadius(s);
		int count = GetVertexCount(s);
		std::uniform_real_distribution<float> radiusDist(radius * 0.7f, radius * 1.3f);

		std::vector<Vec2f> verts;
		verts.reserve(count);
		float step = 2.0f * std::numbers::pi_v<float> / static_cast<float>(count);

		for (int i = 0; i < count; i++)
		{
			float a = step * static_cast<float>(i);
			float r = radiusDist(rng);
			verts.push_back({ r * std::cos(a), r * std::sin(a) });
		}
		return verts;
	}

	static Asteroid Create(Vec2f pos, Vec2f vel, AsteroidSize s, std::mt19937& rng)
	{
		std::uniform_real_distribution<float> angVelDist(-60.0f, 60.0f);
		Asteroid a;
		a.position = pos;
		a.velocity = vel;
		a.size = s;
		a.angularVelocity = angVelDist(rng);
		a.vertices = GenerateVertices(s, rng);
		return a;
	}

	std::vector<Asteroid> Split(std::mt19937& rng) const
	{
		std::vector<Asteroid> children;
		AsteroidSize childSize;
		switch (size)
		{
		case AsteroidSize::Large: childSize = AsteroidSize::Medium; break;
		case AsteroidSize::Medium: childSize = AsteroidSize::Small; break;
		case AsteroidSize::Small: return children;
		}

		std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
		std::uniform_real_distribution<float> speedDist(30.0f, 80.0f);
		int childCount = 2 + (rng() % 2);

		for (int i = 0; i < childCount; i++)
		{
			float a = angleDist(rng) * std::numbers::pi_v<float> / 180.0f;
			float spd = speedDist(rng);
			Vec2f vel = velocity + Vec2f{ std::cos(a) * spd, std::sin(a) * spd };
			children.push_back(Create(position, vel, childSize, rng));
		}
		return children;
	}

	void Update(float dt, float worldW, float worldH)
	{
		angle += angularVelocity * dt;
		position += velocity * dt;

		float halfW = worldW * 0.5f;
		float halfH = worldH * 0.5f;
		if (position.x < -halfW) position.x += worldW;
		if (position.x > halfW) position.x -= worldW;
		if (position.y < -halfH) position.y += worldH;
		if (position.y > halfH) position.y -= worldH;
	}
};
