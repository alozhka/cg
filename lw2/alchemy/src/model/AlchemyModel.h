#pragma once
#include "Element.h"
#include "RecipeBook.h"
#include "shared/Observable.h"
#include "shared/Uuid.h"

#include <algorithm>
#include <random>
#include <ranges>
#include <unordered_set>
#include <vector>

class AlchemyModel : public Observable
{
public:
	explicit AlchemyModel(sf::Vector2f workspaceSize)
		: m_workspaceSize(workspaceSize)
	{
		AddAllElements();
		AddDefaultDiscoveredElements();
		AddRecipes();
	}

	void InsertElement(ElementType type)
	{
		if (!m_discoveredElements.contains(type))
		{
			return;
		}

		Element element{ Uuid::Generate(), type, RandomPos(), m_nextZOrder++ };
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

	void BringToFront(const std::string& id)
	{
		auto it = m_elements.find(id);
		if (it != m_elements.end())
		{
			it->second.SetZOrder(m_nextZOrder++);
			Notify();
		}
	}

	bool TryCombineElements(const std::string& id1, const std::string& id2, sf::Vector2f resultPos)
	{
		auto it1 = m_elements.find(id1);
		auto it2 = m_elements.find(id2);

		if (it1 == m_elements.end() || it2 == m_elements.end())
		{
			return false;
		}

		std::optional<ElementType> combinedType = m_recipeBook.FindRecipeResult(
			it1->second.GetType(),
			it2->second.GetType());

		if (!combinedType.has_value())
		{
			return false;
		}

		m_elements.erase(it1);
		m_elements.erase(it2);

		Element combinedElement{ Uuid::Generate(), *combinedType, resultPos, m_nextZOrder++ };
		m_elements.emplace(combinedElement.GetId(), combinedElement);
		m_discoveredElements.insert(*combinedType);
		Notify();
		return true;
	}

	const std::vector<ElementType>& ListAllElements() const
	{
		return m_allElements;
	}

	std::unordered_set<ElementType> ListDiscoveredElements() const
	{
		return m_discoveredElements;
	}

	std::vector<Element> ListElementsOnWorkspace() const
	{
		std::vector<Element> elements;
		for (const Element& element : m_elements | std::views::values)
		{
			elements.push_back(element);
		}

		std::ranges::sort(elements, [](const Element& a, const Element& b) {
			return a.GetZOrder() < b.GetZOrder();
		});

		return elements;
	}

private:
	void AddAllElements()
	{
		m_allElements = {
			ElementType::Fire,
			ElementType::Earth,
			ElementType::Water,
			ElementType::Air,
			ElementType::Steam,
			ElementType::Lava,
			ElementType::Dust,
			ElementType::Gunpowder,
			ElementType::Mud,
			ElementType::Energy,
			ElementType::Rain,
			ElementType::Metal,
			ElementType::Brick,
			ElementType::Plant,
			ElementType::Swamp,
			ElementType::Glass,
			ElementType::Storm,
			ElementType::Life,
			ElementType::Golem,
			ElementType::Phoenix,
		};
	}

	void AddDefaultDiscoveredElements()
	{
		m_discoveredElements.emplace(ElementType::Earth);
		m_discoveredElements.emplace(ElementType::Water);
		m_discoveredElements.emplace(ElementType::Air);
		m_discoveredElements.emplace(ElementType::Fire);
	}

	void AddRecipes()
	{
		m_recipeBook.AddRecipe(ElementType::Fire, ElementType::Water, ElementType::Steam);
		m_recipeBook.AddRecipe(ElementType::Fire, ElementType::Earth, ElementType::Lava);
		m_recipeBook.AddRecipe(ElementType::Air, ElementType::Earth, ElementType::Dust);
		m_recipeBook.AddRecipe(ElementType::Fire, ElementType::Dust, ElementType::Gunpowder);
		m_recipeBook.AddRecipe(ElementType::Water, ElementType::Earth, ElementType::Mud);
		m_recipeBook.AddRecipe(ElementType::Fire, ElementType::Air, ElementType::Energy);
		m_recipeBook.AddRecipe(ElementType::Water, ElementType::Air, ElementType::Rain);
		m_recipeBook.AddRecipe(ElementType::Fire, ElementType::Lava, ElementType::Metal);
		m_recipeBook.AddRecipe(ElementType::Lava, ElementType::Water, ElementType::Brick);
		m_recipeBook.AddRecipe(ElementType::Rain, ElementType::Earth, ElementType::Plant);
		m_recipeBook.AddRecipe(ElementType::Mud, ElementType::Plant, ElementType::Swamp);
		m_recipeBook.AddRecipe(ElementType::Lava, ElementType::Dust, ElementType::Glass);
		m_recipeBook.AddRecipe(ElementType::Energy, ElementType::Rain, ElementType::Storm);
		m_recipeBook.AddRecipe(ElementType::Swamp, ElementType::Energy, ElementType::Life);
		m_recipeBook.AddRecipe(ElementType::Life, ElementType::Metal, ElementType::Golem);
		m_recipeBook.AddRecipe(ElementType::Life, ElementType::Fire, ElementType::Phoenix);
	}

	static constexpr float MIN_CLAMPED_BOUNDS = 0.2;
	static constexpr float MAX_CLAMPED_BOUNDS = 0.8;

	sf::Vector2f RandomPos() const
	{
		static std::mt19937 gen{ std::random_device{}() };
		float minPos = m_workspaceSize.x * MIN_CLAMPED_BOUNDS;
		float maxPos = m_workspaceSize.x * MAX_CLAMPED_BOUNDS;
		std::uniform_real_distribution<float> distX(minPos, maxPos);
		std::uniform_real_distribution<float> distY(minPos, maxPos);
		return { distX(gen), distY(gen) };
	}

	sf::Vector2f m_workspaceSize;
	int m_nextZOrder = 0;
	RecipeBook m_recipeBook{};
	std::vector<ElementType> m_allElements{};
	std::unordered_set<ElementType> m_discoveredElements{};
	std::unordered_map<std::string, Element> m_elements{};
};
