#pragma once

#include "../model/GameModel.hpp"
#include "InputState.hpp"

#include <string>

class GameViewModel
{
public:
	GameViewModel(float worldW, float worldH)
		: m_model(worldW, worldH)
	{
	}

	void ProcessInput(const InputState& input)
	{
		if (m_model.GetState() == GameState::GameOver) return;

		Ship& ship = m_model.GetShip();

		ship.thrusting = false;

		if (ship.alive)
		{
			if (input.rotateLeft)
			{
				ship.angularVelocity = Ship::ROTATION_SPEED;
			}
			else if (input.rotateRight)
			{
				ship.angularVelocity = -Ship::ROTATION_SPEED;
			}

			if (input.thrust)
			{
				ship.ApplyThrust(1.0f / 60.0f);
			}

			if (input.shoot)
			{
				m_model.FireBullet();
			}
		}
	}

	void Update(float dt)
	{
		m_model.Update(dt);
	}

	void SetWorldSize(float w, float h) { m_model.SetWorldSize(w, h); }

	const Ship& GetShip() const { return m_model.GetShip(); }
	const std::vector<Asteroid>& GetAsteroids() const { return m_model.GetAsteroids(); }
	const std::vector<Bullet>& GetBullets() const { return m_model.GetBullets(); }
	const std::vector<Debris>& GetDebris() const { return m_model.GetDebris(); }
	int GetScore() const { return m_model.GetScore(); }
	int GetLives() const { return m_model.GetLives(); }
	GameState GetGameState() const { return m_model.GetState(); }
	bool IsGameOver() const { return m_model.GetState() == GameState::GameOver; }

	std::string GetWindowTitle() const
	{
		return "Asteroids | Score: " + std::to_string(m_model.GetScore())
			+ " | Lives: " + std::to_string(m_model.GetLives());
	}

	void RestartGame()
	{
		m_model.Reset();
	}

private:
	GameModel m_model;
};
