#pragma once
#include "../model/SeaBattleGame.hpp"

#include <glm/glm.hpp>
#include <vector>

struct ShipDto
{
	ShipType type;
	glm::vec3 position;
	ShipState state;
};

class ShipsViewModel
{
public:
	explicit ShipsViewModel(SeaBattleGamePtr game)
		: m_game(std::move(game))
	{
	}

	std::vector<ShipDto> ListShips() const
	{
		const auto& ships = m_game->GetShips();
		std::vector<ShipDto> result;
		result.reserve(ships.size());
		for (const ShipPtr& ship : ships)
		{
			result.push_back({ ship->GetType(), ship->GetPosition(), ship->GetState() });
		}
		return result;
	}

private:
	SeaBattleGamePtr m_game;
};