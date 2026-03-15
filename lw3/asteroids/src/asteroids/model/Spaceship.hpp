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
	Vec2f m_position{ 0, 0 };
	Vec2f m_velocity{ 0, 0 };
	float m_angle = 0;
	float m_angularVelocity = 0;
};

using SpaceshipPtr = std::shared_ptr<Spaceship>;