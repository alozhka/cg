#pragma once
#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "Collision.hpp"
#include "Debris.hpp"
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
		m_debris.clear();
		m_lives = INITIAL_LIVES;
		m_score = 0;
		m_spaceship->Reset();
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
		if (m_state != GameState::Playing)
		{
			return;
		}

		m_spaceship->Update(dt);
		UpdateBullets(dt);
		UpdateAsteroids(dt);
		UpdateDebris(dt);
		CheckBulletHitsAsteroid();
		CheckAsteroidHitsSpaceship();
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

	std::vector<Debris> ListDebris() const
	{
		return m_debris;
	}

	int GetLives() const
	{
		return m_lives;
	}

	int GetScore() const
	{
		return m_score;
	}

	GameState GetState() const
	{
		return m_state;
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

	void UpdateDebris(float dt)
	{
		for (Debris& d : m_debris)
		{
			d.Update(dt, m_width, m_height);
		}
		std::erase_if(m_debris, [](const Debris& d) { return !d.IsAlive(); });
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

	void CheckAsteroidHitsSpaceship()
	{
		auto shipVertices = m_spaceship->ListWorldVertices();

		for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ++it)
		{
			if (Collision::PolygonsOverlap(shipVertices,
					it->GetPosition(),
					it->ListWorldVertices()))
			{
				auto children = it->Split();
				m_asteroids.erase(it);
				m_asteroids.insert(m_asteroids.end(), children.begin(), children.end());

				auto newDebris = Debris::CreateFromSpaceship(m_spaceship);
				m_debris.insert(m_debris.end(), newDebris.begin(), newDebris.end());

				m_lives--;
				m_spaceship->Reset();

				if (m_lives <= 0)
				{
					m_state = GameState::GameOver;
				}
				return;
			}
		}
	}

	static constexpr int INITIAL_LIVES = 3;
	static constexpr float MAX_SHOOT_COOLDOWN = 0.25;
	static constexpr float ASTEROIDS_SPAWN_AMOUNT = 5;

	GameState m_state = GameState::Idle;
	float m_width, m_height;
	float m_shootCooldown = 0;
	int m_lives = INITIAL_LIVES;
	int m_score = 0;
	SpaceshipPtr m_spaceship;
	std::vector<Bullet> m_bullets{};
	std::vector<Asteroid> m_asteroids;
	std::vector<Debris> m_debris;
};

using AsteroidsGamePtr = std::shared_ptr<AsteroidsGame>;