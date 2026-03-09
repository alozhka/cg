#pragma once

#include "../graphics/Vec2f.hpp"

struct Debris
{
	Vec2f position{ 0, 0 };
	Vec2f velocity{ 0, 0 };
	float angle = 0;
	float angularVelocity = 0;
	float lifetime = 2.0f;
	float maxLifetime = 2.0f;

	Vec2f lineStart{ 0, 0 };
	Vec2f lineEnd{ 0, 0 };

	float GetAlpha() const
	{
		if (maxLifetime <= 0) return 0;
		return lifetime / maxLifetime;
	}

	bool IsAlive() const { return lifetime > 0; }

	void Update(float dt)
	{
		position += velocity * dt;
		angle += angularVelocity * dt;
		lifetime -= dt;
	}
};
