#pragma once

#include "../graphics/Vec2f.hpp"
#include <cmath>
#include <numbers>
#include <vector>

struct Ship
{
	Vec2f position{ 0, 0 };
	Vec2f velocity{ 0, 0 };
	float angle = 0;
	float angularVelocity = 0;
	bool alive = true;
	bool thrusting = false;

	static constexpr float ROTATION_SPEED = 250.0f;
	static constexpr float THRUST_FORCE = 200.0f;
	static constexpr float MAX_SPEED = 300.0f;
	static constexpr float FRICTION = 0.98f;
	static constexpr float ANGULAR_FRICTION = 0.9f;
	static constexpr float SHIP_RADIUS = 15.0f;

	static std::vector<Vec2f> GetShipVertices()
	{
		return {
			{ 0, SHIP_RADIUS },
			{ -SHIP_RADIUS * 0.6f, -SHIP_RADIUS * 0.7f },
			{ 0, -SHIP_RADIUS * 0.4f },
			{ SHIP_RADIUS * 0.6f, -SHIP_RADIUS * 0.7f },
		};
	}

	Vec2f GetNoseDirection() const
	{
		float rad = angle * std::numbers::pi_v<float> / 180.0f;
		return { -std::sin(rad), std::cos(rad) };
	}

	Vec2f GetNosePosition() const
	{
		return position + GetNoseDirection() * SHIP_RADIUS;
	}

	void ApplyThrust(float dt)
	{
		thrusting = true;
		Vec2f dir = GetNoseDirection();
		velocity += dir * (THRUST_FORCE * dt);

		float speed = velocity.Length();
		if (speed > MAX_SPEED)
		{
			velocity = velocity.Normalized() * MAX_SPEED;
		}
	}

	void Update(float dt, float worldW, float worldH)
	{
		if (!alive) return;

		velocity *= FRICTION;
		angularVelocity *= ANGULAR_FRICTION;

		if (std::abs(angularVelocity) < 0.5f) angularVelocity = 0;
		if (velocity.LengthSquared() < 0.01f) velocity = { 0, 0 };

		angle += angularVelocity * dt;
		position += velocity * dt;

		WrapPosition(worldW, worldH);
	}

	void WrapPosition(float worldW, float worldH)
	{
		float halfW = worldW * 0.5f;
		float halfH = worldH * 0.5f;
		if (position.x < -halfW) position.x += worldW;
		if (position.x > halfW) position.x -= worldW;
		if (position.y < -halfH) position.y += worldH;
		if (position.y > halfH) position.y -= worldH;
	}

	void Reset()
	{
		position = { 0, 0 };
		velocity = { 0, 0 };
		angle = 0;
		angularVelocity = 0;
		alive = true;
		thrusting = false;
	}
};
