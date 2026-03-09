#pragma once

#include "../model/GameModel.hpp"
#include "InputState.hpp"
#include "ViewData.hpp"

#include <cmath>
#include <numbers>
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

	ShipViewModel GetShipViewModel() const
	{
		const Ship& ship = m_model.GetShip();

		ShipViewModel vm;
		vm.position = ship.position;
		vm.angle = ship.angle;
		vm.alive = ship.alive;
		vm.thrusting = ship.thrusting;
		vm.vertices = Ship::GetShipVertices();
		vm.flameVertices = BuildFlameVertices();
		return vm;
	}

	std::vector<AsteroidViewModel> GetAsteroidsViewModel() const
	{
		std::vector<AsteroidViewModel> result;
		result.reserve(m_model.GetAsteroids().size());
		for (const auto& a : m_model.GetAsteroids())
		{
			AsteroidViewModel vm;
			vm.position = a.position;
			vm.angle = a.angle;
			vm.vertices = a.vertices;
			result.push_back(std::move(vm));
		}
		return result;
	}

	std::vector<BulletViewModel> GetBulletsViewModel() const
	{
		std::vector<BulletViewModel> result;
		result.reserve(m_model.GetBullets().size());
		for (const auto& b : m_model.GetBullets())
		{
			BulletViewModel vm;
			vm.position = b.position;
			result.push_back(vm);
		}
		return result;
	}

	std::vector<DebrisViewModel> GetDebrisViewModel() const
	{
		std::vector<DebrisViewModel> result;
		result.reserve(m_model.GetDebris().size());
		for (const auto& d : m_model.GetDebris())
		{
			DebrisViewModel vm;
			vm.position = d.position;
			vm.angle = d.angle;
			vm.lineStart = d.lineStart;
			vm.lineEnd = d.lineEnd;
			vm.alpha = d.GetAlpha();
			result.push_back(std::move(vm));
		}
		return result;
	}

	int GetScore() const { return m_model.GetScore(); }
	int GetLives() const { return m_model.GetLives(); }
	GameState GetGameState() const { return m_model.GetState(); }
	bool IsGameOver() const { return m_model.GetState() == GameState::GameOver; }

	std::string GetWindowTitle() const
	{
		return "Asteroids | Score: " + std::to_string(m_model.GetScore())
			+ " | Lives: " + std::to_string(m_model.GetLives());
	}

	void RestartGame() { m_model.Reset(); }

private:
	static std::vector<Vec2f> BuildFlameVertices()
	{
		constexpr float r = Ship::SHIP_RADIUS;
		return {
			{ -r * 0.3f, -r * 0.4f },
			{ 0.0f, -r * 1.1f },
			{ r * 0.3f, -r * 0.4f },
		};
	}

	GameModel m_model;
};
