#pragma once
#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "Collision.hpp"
#include "Randomizer.hpp"
#include "Spaceship.hpp"

#include <random>

class AsteroidsGame
{
public:
	enum class GameState
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

	void StartGame()
	{
		m_asteroids.clear();
		m_bullets.clear();
		SpawnInitialAsteroids();
		m_state = GameState::Playing;
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
		UpdateAsteroids(dt);
		CheckBulletHitsAsteroid();
	}

	SpaceshipPtr GetSpaceship() const
	{
		return m_spaceship;
	}

	std::vector<Bullet> ListBullets() const
	{
		return m_bullets;
	}

	std::vector<Asteroid> ListAsteroids() const
	{
		return m_asteroids;
	}

private:
	void SpawnInitialAsteroids()
	{
		for (int i = 0; i < ASTEROIDS_SPAWN_AMOUNT; i++)
		{
			Vec2f pos{
				Randomizer::RandomRange(-m_width * 0.4, m_width * 0.4),
				Randomizer::RandomRange(-m_height * 0.4, m_height * 0.4)
			};
			m_asteroids.emplace_back(AsteroidSize::Large, pos);
		}
	}

	void UpdateBullets(float dt)
	{
		m_shootCooldown = std::max(m_shootCooldown - dt, 0.0f);

		for (Bullet& bullet : m_bullets)
		{
			bullet.Update(dt, m_width, m_height);
		}

		std::erase_if(m_bullets, [](Bullet& b) { return !b.IsAlive(); });
	}

	void UpdateAsteroids(float dt)
	{
		for (Asteroid& a : m_asteroids)
		{
			a.Update(dt, m_width, m_height);
		}
	}

	void CheckBulletHitsAsteroid()
	{
		std::vector<Asteroid> asteroidsToAdd;
		for (auto asteroidIt = m_asteroids.begin(); asteroidIt != m_asteroids.end();)
		{
			std::vector<Vec2f> verticesInWorld = asteroidIt->ListWorldVertices();
			auto bulletIt = std::ranges::find_if(m_bullets, [&](const Bullet& b) {
				return Collision::PointInPolygon(b.GetPosition(), asteroidIt->GetPosition(), verticesInWorld);
			});

			if (bulletIt == m_bullets.end())
			{
				++asteroidIt;
			}
			else
			{
				std::vector<Asteroid> children = asteroidIt->Split();
				asteroidsToAdd.insert(asteroidsToAdd.end(), children.begin(), children.end());
				m_bullets.erase(bulletIt);
				asteroidIt = m_asteroids.erase(asteroidIt);
			}
		}

		m_asteroids.insert(m_asteroids.end(), asteroidsToAdd.begin(), asteroidsToAdd.end());
	}

	static constexpr float MAX_SHOOT_COOLDOWN = 0.25;
	static constexpr float ASTEROIDS_SPAWN_AMOUNT = 5;

	GameState m_state = GameState::Idle;
	float m_width, m_height;
	float m_shootCooldown = 0;
	SpaceshipPtr m_spaceship;
	std::vector<Bullet> m_bullets{};
	std::vector<Asteroid> m_asteroids;
};

using AsteroidsGamePtr = std::shared_ptr<AsteroidsGame>;