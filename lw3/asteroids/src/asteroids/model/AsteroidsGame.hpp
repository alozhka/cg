#pragma once
#include "Bullet.hpp"
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
		, m_spaceship(std::make_shared<Spaceship>(width, height))
	{
	}

	void FireBullet()
	{
		if (m_shootCooldown > 0)
		{
			return;
		}

		Bullet bullet{
			m_spaceship->GetNosePosition(),
			m_spaceship->GetNoseDirection(),
			m_spaceship->GetVelocity()
		};
		m_bullets.push_back(bullet);
		m_shootCooldown = MAX_SHOOT_COOLDOWN;
	}

	void Update(float dt)
	{
		m_spaceship->Update(dt);
		UpdateBullets(dt);
	}

	SpaceshipPtr GetSpaceship() const
	{
		return m_spaceship;
	}

	std::vector<Bullet> ListBullets() const
	{
		return m_bullets;
	}

private:
	void UpdateBullets(float dt)
	{
		m_shootCooldown = std::max(m_shootCooldown - dt, 0.0f);

		for (Bullet& bullet : m_bullets)
		{
			bullet.Update(dt, m_width, m_height);
		}

		std::erase_if(m_bullets, [](Bullet& b) { return !b.IsAlive(); });
	}

	static constexpr float MAX_SHOOT_COOLDOWN = 0.25;

	GameStatus status = GameStatus::Idle;
	float m_width, m_height;
	float m_shootCooldown = 0;
	SpaceshipPtr m_spaceship;
	std::vector<Bullet> m_bullets{};
};

using AsteroidsGamePtr = std::shared_ptr<AsteroidsGame>;