#pragma once
#include "Element.h"
#include "RecipeBook.h"
#include "shared/Observable.h"
#include "shared/Uuid.h"

#include <ranges>
#include <unordered_set>

class AlchemyModel : public Observable
{
public:
	AlchemyModel()
	{
		AddDefaultElements();
		AddRecipes();
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

	void TryCombineElements(const std::string& id1, const std::string& id2)
	{
		auto it1 = m_elements.find(id1);
		auto it2 = m_elements.find(id2);

		if (it1 == m_elements.end() || it2 == m_elements.end())
		{
			return;
		}

		std::optional<ElementType> combinedType = m_recipeBook.FindRecipeResult(
			it1->second.GetType(),
			it2->second.GetType());

		if (!combinedType.has_value())
		{
			return;
		}

		m_elements.erase(it1);
		m_elements.erase(it2);

		Element combinedElement{ Uuid::Generate(), *combinedType, DEFAULT_ELEMENT_POS };
		m_elements.emplace(combinedElement.GetId(), combinedElement);
		m_discoveredElements.insert(*combinedType);
		Notify();
	}

private:
	void AddDefaultElements()
	{
		m_discoveredElements.emplace(ElementType::Earth);
		m_discoveredElements.emplace(ElementType::Water);
		m_discoveredElements.emplace(ElementType::Air);
		m_discoveredElements.emplace(ElementType::Fire);
	}

	void AddRecipes()
	{
		m_recipeBook.AddRecipe(ElementType::Fire, ElementType::Water, ElementType::Steam);
	}

	static constexpr sf::Vector2f DEFAULT_ELEMENT_POS = { 50, 50 };

	RecipeBook m_recipeBook{};
	std::unordered_set<ElementType> m_discoveredElements{};
	std::unordered_map<std::string, Element> m_elements{};
};
