#pragma once
#include "../model/AsteroidsGame.hpp"

struct AsteroidDto
{
	Vec2f position;
	float angle;
	std::vector<float> vertices;
};

class AsteroidsViewModel
{
public:
	explicit AsteroidsViewModel(const AsteroidsGamePtr& asteroidsGame)
		: m_asteroidsGame(asteroidsGame)
	{
	}

	std::vector<AsteroidDto> ListAsteroids() const
	{
		std::vector<Asteroid> asteroids = m_asteroidsGame->ListAsteroids();
		std::vector<AsteroidDto> newAsteroids;
		newAsteroids.reserve(asteroids.size());

		for (Asteroid& a : asteroids)
		{
			std::vector<Vec2f> vertices = a.ListVertices();
			std::vector<float> newVertices;
			newVertices.reserve(vertices.size());

			for (Vec2f& v : vertices)
			{
				newVertices.push_back(v.x);
				newVertices.push_back(v.y);
			}

			newAsteroids.push_back({ a.GetPosition(),
				a.GetAngle(),
				newVertices });
		}

		return newAsteroids;
	}

private:
	AsteroidsGamePtr m_asteroidsGame;
};