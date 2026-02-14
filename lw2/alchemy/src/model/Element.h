#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>

enum class ElementType
{
	Fire = 0,
	Air = 1,
	Earth = 2,
	Water = 3,
};

inline std::wstring ElementTypeName(ElementType type)
{
	switch (type)
	{
	case ElementType::Fire:
		return L"Огонь";
	case ElementType::Air:
		return L"Воздух";
	case ElementType::Earth:
		return L"Земля";
	case ElementType::Water:
		return L"Вода";
	default:
		throw std::invalid_argument("Invalid element type");
	}
}

class Element
{
public:
	Element(const std::string& id, ElementType type, sf::Vector2f pos)
		: m_id(id)
		, m_type(type)
		, m_pos(pos)
	{
	}

private:
	std::string m_id;
	ElementType m_type;
	sf::Vector2f m_pos;
};
