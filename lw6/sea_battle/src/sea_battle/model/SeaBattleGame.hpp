#pragma once
#include "Ship.hpp"
#include "Torpedo.hpp"

#include <vector>

class SeaBattleGame
{
private:
	std::vector<ShipPtr> m_ships;
	TorpedoPtr m_torpedo;
};
