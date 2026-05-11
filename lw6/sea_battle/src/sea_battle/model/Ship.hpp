#pragma once
#include <memory>

enum class ShipState
{
	Alive,
	Drowning
};

class Ship;
using ShipPtr = std::shared_ptr<Ship>;

class Ship
{
private:
	ShipState m_state = ShipState::Alive;
};

class Cruiser : public Ship
{
};

class Battleship : public Ship
{
};

class Frigate : public Ship
{
};