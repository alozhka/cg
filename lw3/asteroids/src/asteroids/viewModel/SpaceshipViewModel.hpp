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

		for (Vec2f& vertex : vertices)
		{
			newVertices.push_back(vertex.x);
			newVertices.push_back(vertex.y);
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

private:
	SpaceshipPtr m_spaceship;
};