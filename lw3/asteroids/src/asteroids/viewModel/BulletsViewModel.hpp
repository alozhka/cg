#pragma once
#include "../model/AsteroidsGame.hpp"

class BulletsViewModel
{
public:
	explicit BulletsViewModel(const AsteroidsGamePtr& asteroidsGame)
		: m_asteroidsGame(asteroidsGame)
	{
	}

	static std::vector<float> ListBulletVertices()
	{
		std::vector<Vec2f> vertices = Bullet::ListVertices();
		std::vector<float> newVertices;

		for (Vec2f& v : vertices)
		{
			newVertices.push_back(v.x);
			newVertices.push_back(v.y);
		}

		return newVertices;
	}

	std::vector<Vec2f> ListBulletsPositions() const
	{
		std::vector<Bullet> bullets = m_asteroidsGame->ListBullets();
		std::vector<Vec2f> positions;
		positions.reserve(bullets.size());

		for (Bullet& bullet : bullets)
		{
			positions.push_back(bullet.GetPosition());
		}

		return positions;
	}

private:
	AsteroidsGamePtr m_asteroidsGame;
};