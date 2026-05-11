#pragma once
#include <glm/glm.hpp>
#include <memory>

enum class ShipState
{
	Alive,
	Drowning
};

enum class ShipType
{
	Cruiser,
	Battleship,
	Frigate
};

inline float GetShipSpeed(ShipType type)
{
	switch (type)
	{
	case ShipType::Cruiser:
		return 3;
	case ShipType::Battleship:
		return 1.8;
	case ShipType::Frigate:
		return 4.5;
	}

	throw std::invalid_argument("Unsupported ship type");
}

class Ship;
using ShipPtr = std::shared_ptr<Ship>;

class Ship
{
public:
	Ship(ShipType type, const glm::vec3& position)
		: m_type(type)
		, m_position(position)
		, m_speed(GetShipSpeed(type))
	{
	}

	void Update(float dt)
	{
		m_position.x += m_speed * dt;
	}

	ShipType GetType() const
	{
		return m_type;
	}
	const glm::vec3& GetPosition() const
	{
		return m_position;
	}
	float GetSpeed() const
	{
		return m_speed;
	}
	ShipState GetState() const
	{
		return m_state;
	}

	bool IsOutOfBounds(float maxX) const
	{
		return m_position.x > maxX;
	}

private:
	ShipType m_type;
	glm::vec3 m_position;
	float m_speed;
	ShipState m_state = ShipState::Alive;
};