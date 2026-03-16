#pragma once
#include "../../graphics/Vec2f.hpp"
#include "Coordinates.hpp"
#include "Randomizer.hpp"
#include "Spaceship.hpp"

#include <vector>

class Debris
{
public:
	Debris(Vec2f position, Vec2f initialVelocity, float lifetime)
		: m_position(position)
		, m_velocity(initialVelocity + Vec2f{ Randomizer::RandomRange(-SPREAD_VELOCITY, SPREAD_VELOCITY), Randomizer::RandomRange(-SPREAD_VELOCITY, SPREAD_VELOCITY) })
		, m_lifetime(lifetime)
	{
	}

	void Update(float dt, float width, float height)
	{
		m_lifetime = std::max(m_lifetime - dt, 0.0f);
		m_position += m_velocity * dt;
		m_position = Coordinates::Wrap(m_position, width, height);
		m_angle += m_angularVelocity * dt;
		m_angle = Coordinates::WrapAngle(m_angle);
	}

	bool IsAlive() const
	{
		return m_lifetime > 0;
	}

	Vec2f GetPosition() const
	{
		return m_position;
	}

	float GetAngle() const
	{
		return m_angle;
	}

	static std::vector<Vec2f> ListVertices()
	{
		return {
			{ -4, 0 },
			{ 4, 0 }
		};
	}

	static std::vector<Debris> CreateFromSpaceship(const SpaceshipPtr& spaceship)
	{
		Vec2f position = spaceship->GetPosition();
		Vec2f velocity = spaceship->GetVelocity();
		std::vector<Debris> result;
		result.reserve(DEBRIS_AMOUNT);

		for (int i = 0; i < DEBRIS_AMOUNT; ++i)
		{
			result.emplace_back(position, velocity, INITIAL_LIFETIME);
		}

		return result;
	}

private:
	static constexpr int DEBRIS_AMOUNT = 10;
	static constexpr float INITIAL_LIFETIME = 1.8f;
	static constexpr float SPREAD_VELOCITY = 120;
	static constexpr float ANGULAR_SPREAD = 200;

	Vec2f m_position;
	Vec2f m_velocity;
	float m_angle = Randomizer::RandomRange(0, 360);
	float m_angularVelocity = Randomizer::RandomRange(-ANGULAR_SPREAD, ANGULAR_SPREAD);
	float m_lifetime = 0;
};
