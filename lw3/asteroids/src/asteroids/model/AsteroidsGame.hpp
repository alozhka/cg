#pragma once
#include "Spaceship.hpp"

class AsteroidsGame
{
public:
	enum class GameStatus
	{
		Idle = 0,
		Playing = 1,
		GameOver = 2,
	};

	AsteroidsGame(float width, float height)
		: m_width(width)
		, m_height(height)
	{
	}

	SpaceshipPtr GetSpaceship() const
	{
		return m_spaceship;
	}

private:
	float m_width, m_height;
	GameStatus status = GameStatus::Idle;
	SpaceshipPtr m_spaceship = std::make_shared<Spaceship>();
};