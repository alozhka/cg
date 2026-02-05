#pragma once
#include "SFML/System/Vector2.hpp"
#include <cmath>

constexpr float GRAVITY = -500.0f;

class BouncingObject
{
public:
	float GetY() const
	{
		return m_y;
	}

	BouncingObject(sf::Vector2f startPos, float speed)
		: m_y0(startPos.y)
		, m_y(startPos.y)
		, m_speed(speed)
		, m_period(2 * speed / GRAVITY)
	{
	}

	void update(float t)
	{
		float dt = std::fmod(t, m_period);
		// y = y0 + v0*t - 0,5*g*t^2
		float y = m_y0 + m_speed * dt - 0.5f * GRAVITY * dt * dt;
		m_y = y;
	}

private:
	float m_y0, m_y;
	float m_speed;
	float m_period;
};
