#pragma once

#include "Asteroid.hpp"
#include "Bullet.hpp"
#include "CollisionDetector.hpp"
#include "Debris.hpp"
#include "Ship.hpp"

#include <algorithm>
#include <random>
#include <vector>

enum class GameState
{
	Playing,
	Respawning,
	GameOver
};

class GameModel
{
public:
	GameModel(float worldW, float worldH)
		: m_worldWidth(worldW)
		, m_worldHeight(worldH)
		, m_rng(std::random_device{}())
	{
		Reset();
	}

	void Reset()
	{
		m_ship.Reset();
		m_asteroids.clear();
		m_bullets.clear();
		m_debris.clear();
		m_score = 0;
		m_lives = 3;
		m_state = GameState::Playing;
		m_respawnTimer = 0;
		m_shootCooldown = 0;
		SpawnAsteroids(5);
	}

	void Update(float dt)
	{
		if (m_state == GameState::GameOver) return;

		m_shootCooldown -= dt;

		if (m_state == GameState::Respawning)
		{
			m_respawnTimer -= dt;
			if (m_respawnTimer <= 0)
			{
				m_ship.Reset();
				m_state = GameState::Playing;
			}
		}

		m_ship.Update(dt, m_worldWidth, m_worldHeight);

		for (auto& a : m_asteroids)
		{
			a.Update(dt, m_worldWidth, m_worldHeight);
		}

		for (auto& b : m_bullets)
		{
			b.Update(dt, m_worldWidth, m_worldHeight);
		}

		for (auto& d : m_debris)
		{
			d.Update(dt);
		}

		std::erase_if(m_bullets, [](const Bullet& b) { return !b.IsAlive(); });
		std::erase_if(m_debris, [](const Debris& d) { return !d.IsAlive(); });

		CheckBulletAsteroidCollisions();

		if (m_ship.alive)
		{
			CheckShipAsteroidCollisions();
		}

		if (m_asteroids.empty() && m_state == GameState::Playing)
		{
			SpawnAsteroids(5 + m_score / 500);
		}
	}

	void FireBullet()
	{
		if (!m_ship.alive) return;
		if (m_shootCooldown > 0) return;

		int activeBullets = static_cast<int>(m_bullets.size());
		if (activeBullets >= Bullet::MAX_ON_SCREEN) return;

		Bullet b;
		b.position = m_ship.GetNosePosition();
		b.velocity = m_ship.GetNoseDirection() * Bullet::SPEED + m_ship.velocity;
		b.lifetime = Bullet::MAX_LIFETIME;
		m_bullets.push_back(b);
		m_shootCooldown = Bullet::COOLDOWN;
	}

	const Ship& GetShip() const { return m_ship; }
	Ship& GetShip() { return m_ship; }
	const std::vector<Asteroid>& GetAsteroids() const { return m_asteroids; }
	const std::vector<Bullet>& GetBullets() const { return m_bullets; }
	const std::vector<Debris>& GetDebris() const { return m_debris; }
	int GetScore() const { return m_score; }
	int GetLives() const { return m_lives; }
	GameState GetState() const { return m_state; }
	float GetWorldWidth() const { return m_worldWidth; }
	float GetWorldHeight() const { return m_worldHeight; }

	void SetWorldSize(float w, float h) { m_worldWidth = w; m_worldHeight = h; }

private:
	void SpawnAsteroids(int count)
	{
		std::uniform_real_distribution<float> angleDist(0.0f, 360.0f);
		std::uniform_real_distribution<float> speedDist(20.0f, 60.0f);

		float halfW = m_worldWidth * 0.5f;
		float halfH = m_worldHeight * 0.5f;

		for (int i = 0; i < count; i++)
		{
			Vec2f pos;
			int edge = m_rng() % 4;
			std::uniform_real_distribution<float> xDist(-halfW, halfW);
			std::uniform_real_distribution<float> yDist(-halfH, halfH);

			switch (edge)
			{
			case 0: pos = { xDist(m_rng), halfH }; break;
			case 1: pos = { xDist(m_rng), -halfH }; break;
			case 2: pos = { -halfW, yDist(m_rng) }; break;
			case 3: pos = { halfW, yDist(m_rng) }; break;
			}

			float a = angleDist(m_rng) * std::numbers::pi_v<float> / 180.0f;
			float spd = speedDist(m_rng);
			Vec2f vel{ std::cos(a) * spd, std::sin(a) * spd };

			m_asteroids.push_back(Asteroid::Create(pos, vel, AsteroidSize::Large, m_rng));
		}
	}

	void CheckBulletAsteroidCollisions()
	{
		std::vector<Asteroid> newAsteroids;

		for (auto bulletIt = m_bullets.begin(); bulletIt != m_bullets.end();)
		{
			bool bulletHit = false;

			for (auto astIt = m_asteroids.begin(); astIt != m_asteroids.end(); ++astIt)
			{
				auto worldVerts = CollisionDetector::TransformPolygon(
					astIt->vertices, astIt->position, astIt->angle);

				if (CollisionDetector::PointInPolygon(bulletIt->position, worldVerts))
				{
					m_score += Asteroid::GetScore(astIt->size);
					auto children = astIt->Split(m_rng);
					newAsteroids.insert(newAsteroids.end(), children.begin(), children.end());
					m_asteroids.erase(astIt);
					bulletHit = true;
					break;
				}
			}

			if (bulletHit)
			{
				bulletIt = m_bullets.erase(bulletIt);
			}
			else
			{
				++bulletIt;
			}
		}

		m_asteroids.insert(m_asteroids.end(), newAsteroids.begin(), newAsteroids.end());
	}

	void CheckShipAsteroidCollisions()
	{
		auto shipVerts = Ship::GetShipVertices();
		auto worldShipVerts = CollisionDetector::TransformPolygon(
			shipVerts, m_ship.position, m_ship.angle);

		for (auto it = m_asteroids.begin(); it != m_asteroids.end(); ++it)
		{
			auto worldAstVerts = CollisionDetector::TransformPolygon(
				it->vertices, it->position, it->angle);

			if (CollisionDetector::PolygonsIntersect(worldShipVerts, worldAstVerts))
			{
				auto children = it->Split(m_rng);
				m_asteroids.erase(it);
				m_asteroids.insert(m_asteroids.end(), children.begin(), children.end());

				DestroyShip();
				return;
			}
		}
	}

	void DestroyShip()
	{
		m_ship.alive = false;
		m_lives--;

		CreateShipDebris();

		if (m_lives <= 0)
		{
			m_state = GameState::GameOver;
		}
		else
		{
			m_state = GameState::Respawning;
			m_respawnTimer = 2.0f;
		}
	}

	void CreateShipDebris()
	{
		auto verts = Ship::GetShipVertices();
		std::uniform_real_distribution<float> velDist(-50.0f, 50.0f);
		std::uniform_real_distribution<float> angDist(-180.0f, 180.0f);

		for (size_t i = 0; i < verts.size(); i++)
		{
			size_t next = (i + 1) % verts.size();

			float rad = m_ship.angle * std::numbers::pi_v<float> / 180.0f;
			float c = std::cos(rad), s = std::sin(rad);

			Vec2f v1 = { verts[i].x * c - verts[i].y * s, verts[i].x * s + verts[i].y * c };
			Vec2f v2 = { verts[next].x * c - verts[next].y * s, verts[next].x * s + verts[next].y * c };

			Debris d;
			d.position = m_ship.position;
			d.velocity = m_ship.velocity + Vec2f{ velDist(m_rng), velDist(m_rng) };
			d.angularVelocity = angDist(m_rng);
			d.lineStart = v1;
			d.lineEnd = v2;
			d.lifetime = 2.0f;
			d.maxLifetime = 2.0f;
			m_debris.push_back(d);
		}
	}

	Ship m_ship;
	std::vector<Asteroid> m_asteroids;
	std::vector<Bullet> m_bullets;
	std::vector<Debris> m_debris;

	int m_score = 0;
	int m_lives = 3;
	GameState m_state = GameState::Playing;
	float m_respawnTimer = 0;
	float m_shootCooldown = 0;

	float m_worldWidth;
	float m_worldHeight;
	std::mt19937 m_rng;
};
