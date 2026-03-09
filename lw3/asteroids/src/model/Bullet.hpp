#pragma once

#include "../graphics/Vec2f.hpp"

struct Bullet
{
	Vec2f position{ 0, 0 };
	Vec2f velocity{ 0, 0 };
	float lifetime = 2.0f;

	static constexpr float SPEED = 350.0f;
	static constexpr float MAX_LIFETIME = 2.0f;
	static constexpr float COOLDOWN = 0.25f;
	static constexpr int MAX_ON_SCREEN = 5;

	bool IsAlive() const { return lifetime > 0; }

	void Update(float dt, float worldW, float worldH)
	{
		position += velocity * dt;
		lifetime -= dt;

		float halfW = worldW * 0.5f;
		float halfH = worldH * 0.5f;
		if (position.x < -halfW) position.x += worldW;
		if (position.x > halfW) position.x -= worldW;
		if (position.y < -halfH) position.y += worldH;
		if (position.y > halfH) position.y -= worldH;
	}
};
