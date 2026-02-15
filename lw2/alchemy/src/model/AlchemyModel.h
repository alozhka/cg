#pragma once
#include "Element.h"
#include "shared/Observable.h"
#include "shared/Uuid.h"

#include <iostream>
#include <unordered_set>

class AlchemyModel : Observable
{
public:
	AlchemyModel()
	{
		m_discoveredElements.emplace(ElementType::Earth);
		m_discoveredElements.emplace(ElementType::Water);
		m_discoveredElements.emplace(ElementType::Air);
		m_discoveredElements.emplace(ElementType::Fire);
	}

	std::unordered_set<ElementType> ListDiscoveredElements() const
	{
		return m_discoveredElements;
	}

	void InsertElement(ElementType type)
	{
		Element element{ Uuid::Generate(), type, DEFAULT_ELEMENT_POS };
		m_elements.emplace(element.GetId(), element);
		Notify();
	}

private:
	static constexpr sf::Vector2f DEFAULT_ELEMENT_POS = { 50, 50 };

	std::unordered_set<ElementType> m_discoveredElements;
	std::unordered_map<std::string, Element> m_elements;
};
