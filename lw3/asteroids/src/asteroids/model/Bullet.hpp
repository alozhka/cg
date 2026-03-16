#pragma once
#include "../../graphics/Vec2f.hpp"
#include "Coordinates.hpp"
#include <vector>

class Bullet
{
public:
	explicit Bullet(Vec2f position, Vec2f direction, Vec2f initialVelocity)
		: m_position(position)
		, m_velocity(initialVelocity + direction * INITIAL_SPEED)
	{
	}

	void Update(float dt, float width, float height)
	{
		m_lifetime = std::max(m_lifetime - dt, 0.0f);

		m_position += m_velocity * dt;
		m_position = Coordinates::Wrap(m_position, width, height);
	}

	bool IsAlive() const
	{
		return m_lifetime > 0;
	}

	static std::vector<Vec2f> ListVertices()
	{
		return {
			{ -THICKNESS, -THICKNESS },
			{ THICKNESS, -THICKNESS },
			{ THICKNESS, THICKNESS },
			{ -THICKNESS, -THICKNESS },
			{ THICKNESS, THICKNESS },
			{ -THICKNESS, THICKNESS }
		};
	}

	Vec2f GetPosition() const
	{
		return m_position;
	}

private:
	static constexpr float INITIAL_SPEED = 500;
	static constexpr float MAX_LIFETIME_SECONDS = 2;
	static constexpr float THICKNESS = 2;

	float m_lifetime = MAX_LIFETIME_SECONDS;
	Vec2f m_position;
	Vec2f m_velocity;
};