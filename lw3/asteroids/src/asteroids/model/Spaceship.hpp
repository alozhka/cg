#pragma once
#include "../../graphics/Vec2f.hpp"

#include <memory>
#include <vector>

class Spaceship
{
public:
	Spaceship(float worldWidth, float worldHeight)
		: m_worldWidth(worldWidth)
		, m_worldHeight(worldHeight)
	{
	}

	void BeginThrust()
	{
		m_isThrusting = true;
	}

	void FinishThrust()
	{
		m_isThrusting = false;
	}

	void ApplyThrust(float dt)
	{
		m_isThrusting = true;
		Vec2f direction = GetNoseDirection();
		m_velocity += direction * THRUST_FORCE * dt;

		if (m_velocity.GetLength() > MAX_SPEED)
		{
			m_velocity = m_velocity.GetNormalized() * MAX_SPEED;
		}
	}

	void ApplyLeftRotation()
	{
		m_angularVelocity = MAX_ANGLE_SPEED;
	}

	void ApplyRightRotation()
	{
		m_angularVelocity = -MAX_ANGLE_SPEED;
	}

	void Update(float dt)
	{
		if (m_isThrusting)
		{
			ApplyThrust(dt);
		}

		UpdatePosition(dt);
		UpdateAnglePosition(dt);
	}

	void UpdatePosition(float dt)
	{
		m_velocity *= FRICTION;
		m_position += m_velocity * dt;
		WrapPosition();
	}

	void UpdateAnglePosition(float dt)
	{
		m_angularVelocity *= ANGLE_FRICTION;
		m_angleInDegrees += m_angularVelocity * dt;
		if (m_angleInDegrees >= 360)
		{
			m_angleInDegrees -= 360;
		}
	}

	std::vector<Vec2f> ListVertices() const
	{
		return {
			{ 0, 15 },
			{ -8, -9.4 },
			{ 0, -6 },
			{ 8, -9.4 }
		};
	}

	std::vector<Vec2f> ListFlameVertices() const
	{
		return {
			{ -4.5, -6 },
			{ 0, -15 },
			{ 4.5, -6 }
		};
	}

	Vec2f GetPosition() const
	{
		return m_position;
	}

	float GetAngle() const
	{
		return m_angleInDegrees;
	}

	bool IsThrusting() const
	{
		return m_isThrusting;
	}

private:
	void WrapPosition()
	{
		float halfWidth = m_worldWidth * 0.5f;
		float halfHeight = m_worldHeight * 0.5f;

		if (m_position.x < -halfWidth)
		{
			m_position.x += m_worldWidth;
		}
		if (m_position.x > halfWidth)
		{
			m_position.x -= m_worldWidth;
		}
		if (m_position.y < -halfHeight)
		{
			m_position.y += m_worldHeight;
		}
		if (m_position.y > halfHeight)
		{
			m_position.y -= m_worldHeight;
		}
	}

	Vec2f GetNoseDirection() const
	{
		float radians = m_angleInDegrees * std::numbers::pi_v<float> / 180;
		return { -std::sin(radians), std::cos(radians) };
	}

	static constexpr float THRUST_FORCE = 600;
	static constexpr float MAX_SPEED = 900;
	static constexpr float MAX_ANGLE_SPEED = 250;
	static constexpr float FRICTION = 0.98;
	static constexpr float ANGLE_FRICTION = 0.9;

	bool m_isThrusting = false;
	Vec2f m_position{ 0, 0 };
	Vec2f m_velocity{ 0, 0 };
	float m_angleInDegrees = 0;
	float m_angularVelocity = 0;
	float m_worldWidth, m_worldHeight;
};

using SpaceshipPtr = std::shared_ptr<Spaceship>;