#pragma once
#include "Element.h"

#include <functional>
#include <optional>
#include <unordered_map>
#include <utility>

struct ElementPairHash
{
	std::size_t operator()(const std::pair<ElementType, ElementType>& pair) const noexcept
	{
		auto h1 = std::hash<int>{}(static_cast<int>(pair.first));
		auto h2 = std::hash<int>{}(static_cast<int>(pair.second));
		return h1 ^ (h2 << 16);
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

private:
	static std::pair<ElementType, ElementType> MakeKey(ElementType first, ElementType second)
	{
		if (first > second)
		{
			return { second, first };
		}
		return { first, second };
	}

	std::unordered_map<std::pair<ElementType, ElementType>, ElementType, ElementPairHash> m_recipes{};
};
