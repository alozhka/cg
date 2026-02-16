#pragma once
#include "Element.h"

#include <functional>
#include <map>
#include <optional>
#include <unordered_set>
#include <utility>

struct ElementTypeHash
{
	std::size_t operator()(ElementType type) const noexcept
	{
		return std::hash<int>{}(static_cast<int>(type));
	}
};

class RecipeBook
{
public:
	void AddRecipe(ElementType first, ElementType second, ElementType result)
	{
		m_recipes[MakeKey(first, second)] = result;
	}

	std::optional<ElementType> FindRecipeResult(ElementType first, ElementType second) const
	{
		auto it = m_recipes.find(MakeKey(first, second));
		if (it != m_recipes.end())
		{
			return it->second;
		}
		return std::nullopt;
	}

	static const std::unordered_set<ElementType, ElementTypeHash>& ListElements()
	{
		return m_elements;
	}

private:
	static std::pair<ElementType, ElementType> MakeKey(ElementType first, ElementType second)
	{
		if (first > second)
		{
			return { second, first };
		}
		return { first, second };
	}

	// TODO: на ordered map
	std::map<std::pair<ElementType, ElementType>, ElementType> m_recipes{};
	// TODO: на вектор
	inline static std::unordered_set<ElementType, ElementTypeHash> m_elements{
		ElementType::Earth,
		ElementType::Water,
		ElementType::Air,
		ElementType::Fire,
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
};
