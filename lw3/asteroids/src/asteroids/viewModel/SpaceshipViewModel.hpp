#pragma once
#include "../model/Spaceship.hpp"

class SpaceshipViewModel
{
public:
	explicit SpaceshipViewModel(const SpaceshipPtr& spaceship)
		: m_spaceship(spaceship)
	{
	}

	std::vector<float> ListVertices() const
	{
		std::vector<Vec2f> vertices = m_spaceship->ListVertices();
		std::vector<float> newVertices;

		for (Vec2f& v : vertices)
		{
			newVertices.push_back(v.x);
			newVertices.push_back(v.y);
		}

		return newVertices;
	}

	std::vector<float> ListFlameVertices() const
	{
		std::vector<Vec2f> vertices = m_spaceship->ListFlameVertices();
		std::vector<float> newVertices;

		for (Vec2f& v : vertices)
		{
			newVertices.push_back(v.x);
			newVertices.push_back(v.y);
		}

		return newVertices;
	}

	Vec2f GetPosition() const
	{
		return m_spaceship->GetPosition();
	}

	float GetAngle() const
	{
		return m_spaceship->GetAngle();
	}

	bool IsThrusting() const
	{
		return m_spaceship->IsThrusting();
	}

private:
	SpaceshipPtr m_spaceship;
};