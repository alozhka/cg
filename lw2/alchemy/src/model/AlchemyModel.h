#pragma once
#include "Element.h"

#include <unordered_set>

class AlchemyModel
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

private:
	std::unordered_set<ElementType> m_discoveredElements;
};
