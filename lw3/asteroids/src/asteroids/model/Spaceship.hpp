#pragma once
#include "../../graphics/Vec2f.hpp"

#include <memory>
#include <vector>

class Spaceship
{
public:
	Spaceship()
	{
	}

	void ApplyThrust(float dt)
	{
		Vec2f direction = GetNoseDirection();
		m_velocity += direction * THRUST_FORCE * dt;

		if (m_velocity.GetLength() > MAX_SPEED)
		{
			m_velocity = m_velocity.GetNormalized() * MAX_SPEED;
		}
	}

	void Update(float dt)
	{
		m_velocity *= FRICTION;
		m_position += m_velocity * dt;
	}

	std::vector<Vec2f> ListVertices()
	{
		return {
			{ 0, 15 },
			{ -8, -9.4 },
			{ 0, -6 },
			{ 8, -9.4 }
		};
	}

	Vec2f GetPosition() const
	{
		return m_position;
	}

	float GetAngle() const
	{
		return m_angle;
	}

private:
	Vec2f GetNoseDirection() const
	{
		float radians = m_angle * std::numbers::pi_v<float> / 180;
		return { -std::sin(radians), std::cos(radians) };
	}

	static constexpr float THRUST_FORCE = 2;
	static constexpr float MAX_SPEED = 10;
	static constexpr float FRICTION = 0.98;
	static constexpr float ANGLE_FRICTION = 0.8;

	bool m_isThrusting = false;
	Vec2f m_position{ 0, 0 };
	Vec2f m_velocity{ 0, 0 };
	float m_angle = 0;
	float m_angularVelocity = 0;
};

using SpaceshipPtr = std::shared_ptr<Spaceship>;