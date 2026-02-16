#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>

enum class ElementType
{
	Fire = 0,
	Earth = 1,
	Water = 2,
	Air = 3,
	Steam = 4,
	Lava = 5,
	Dust = 6,
	Gunpowder = 7,
	Mud = 8,
	Energy = 9,
	Rain = 10,
	Metal = 11,
	Brick = 12,
	Plant = 13,
	Swamp = 14,
	Glass = 15,
	Storm = 16,
	Life = 17,
	Golem = 18,
	Phoenix = 19,
};

static std::wstring ElementTypeToString(ElementType type)
{
	switch (type)
	{
	case ElementType::Fire:
		return L"Огонь";
	case ElementType::Earth:
		return L"Земля";
	case ElementType::Water:
		return L"Вода";
	case ElementType::Air:
		return L"Воздух";
	case ElementType::Steam:
		return L"Пар";
	case ElementType::Lava:
		return L"Лава";
	case ElementType::Dust:
		return L"Пыль";
	case ElementType::Gunpowder:
		return L"Порох";
	case ElementType::Mud:
		return L"Грязь";
	case ElementType::Energy:
		return L"Энергия";
	case ElementType::Rain:
		return L"Дождь";
	case ElementType::Metal:
		return L"Металл";
	case ElementType::Brick:
		return L"Кирпич";
	case ElementType::Plant:
		return L"Растение";
	case ElementType::Swamp:
		return L"Болото";
	case ElementType::Glass:
		return L"Стекло";
	case ElementType::Storm:
		return L"Шторм";
	case ElementType::Life:
		return L"Жизнь";
	case ElementType::Golem:
		return L"Голем";
	case ElementType::Phoenix:
		return L"Феникс";
	default:
		return L"???";
	}
}

static ElementType ConvertToElementType(const std::wstring& type)
{
	if (type == L"Огонь")
	{
		return ElementType::Fire;
	}
	if (type == L"Земля")
	{
		return ElementType::Earth;
	}
	if (type == L"Вода")
	{
		return ElementType::Water;
	}
	if (type == L"Воздух")
	{
		return ElementType::Air;
	}
	if (type == L"Пар")
	{
		return ElementType::Steam;
	}
	if (type == L"Лава")
	{
		return ElementType::Lava;
	}
	if (type == L"Пыль")
	{
		return ElementType::Dust;
	}
	if (type == L"Порох")
	{
		return ElementType::Gunpowder;
	}
	if (type == L"Грязь")
	{
		return ElementType::Mud;
	}
	if (type == L"Энергия")
	{
		return ElementType::Energy;
	}
	if (type == L"Дождь")
	{
		return ElementType::Rain;
	}
	if (type == L"Металл")
	{
		return ElementType::Metal;
	}
	if (type == L"Кирпич")
	{
		return ElementType::Brick;
	}
	if (type == L"Растение")
	{
		return ElementType::Plant;
	}
	if (type == L"Болото")
	{
		return ElementType::Swamp;
	}
	if (type == L"Стекло")
	{
		return ElementType::Glass;
	}
	if (type == L"Шторм")
	{
		return ElementType::Storm;
	}
	if (type == L"Жизнь")
	{
		return ElementType::Life;
	}
	if (type == L"Голем")
	{
		return ElementType::Golem;
	}
	if (type == L"Феникс")
	{
		return ElementType::Phoenix;
	}

	throw std::invalid_argument("Unsupported element type");
}

class Element
{
public:
	Element(const std::string& id, ElementType type, sf::Vector2f pos, int zOrder)
		: m_id(id)
		, m_type(type)
		, m_pos(pos)
		, m_zOrder(zOrder)
	{
	}

	std::string GetId() const
	{
		return m_id;
	}

	ElementType GetType() const
	{
		return m_type;
	}

	sf::Vector2f GetPosition() const
	{
		return m_pos;
	}

	void SetPosition(sf::Vector2f pos)
	{
		m_pos = pos;
	}

	int GetZOrder() const
	{
		return m_zOrder;
	}

	void SetZOrder(int zOrder)
	{
		m_zOrder = zOrder;
	}

private:
	std::string m_id;
	ElementType m_type;
	sf::Vector2f m_pos;
	int m_zOrder;
};
