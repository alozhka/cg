#pragma once
#include "Element.h"
#include "shared/Observable.h"
#include "shared/Uuid.h"

#include <ranges>
#include <unordered_set>

class AlchemyModel : public Observable
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

	void MoveElement(const std::string& id, sf::Vector2f newPos)
	{
		auto it = m_elements.find(id);
		if (it != m_elements.end())
		{
			it->second.SetPosition(newPos);
			Notify();
		}
	}

	void RemoveElement(const std::string& id)
	{
		if (m_elements.erase(id))
		{
			Notify();
		}
	}

	std::vector<Element> ListElementsOnWorkspace() const
	{
		std::vector<Element> elements;
		for (const Element& element : m_elements | std::views::values)
		{
			elements.push_back(element);
		}

		return elements;
	}

private:
	static constexpr sf::Vector2f DEFAULT_ELEMENT_POS = { 50, 50 };

	std::unordered_set<ElementType> m_discoveredElements;
	std::unordered_map<std::string, Element> m_elements;
};
