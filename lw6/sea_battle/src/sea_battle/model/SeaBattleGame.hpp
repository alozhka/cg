#pragma once
#include "Ship.hpp"
#include "Torpedo.hpp"

#include <graphics/shared/Randomizer.hpp>
#include <vector>

class SeaBattleGame;
using SeaBattleGamePtr = std::shared_ptr<SeaBattleGame>;

class SeaBattleGame
{
public:
	static constexpr float SPAWN_X = -30;
	static constexpr float DESPAWN_X = 30;
	static constexpr float WATER_Y = 0;
	static constexpr float MIN_Z = -8;
	static constexpr float MAX_Z = 8;
	static constexpr float SPAWN_INTERVAL = 2.5;

	void Update(float dt)
	{
		m_spawnTimer -= dt;
		if (m_spawnTimer <= 0)
		{
			SpawnRandomShip();
			m_spawnTimer = SPAWN_INTERVAL;
		}

		for (auto& ship : m_ships)
		{
			ship->Update(dt);
		}

		std::erase_if(m_ships, [](const ShipPtr& ship) {
			return ship->IsOutOfBounds(DESPAWN_X);
		});
	}

	const std::vector<ShipPtr>& GetShips() const { return m_ships; }

private:
	void SpawnRandomShip()
	{
		const glm::vec3 position{ SPAWN_X, WATER_Y, Randomizer::RandomRange(MIN_Z, MAX_Z) };
		const auto type = static_cast<ShipType>(static_cast<int>(Randomizer::RandomRange(0, 3)));

		m_ships.push_back(std::make_shared<Ship>(type, position));
	}

	std::vector<ShipPtr> m_ships;
	TorpedoPtr m_torpedo;
	float m_spawnTimer = 0.0f;
};