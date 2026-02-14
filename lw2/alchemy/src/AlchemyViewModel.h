#pragma once
#include "model/AlchemyModel.h"
#include "model/Element.h"
#include "model/RecipeBook.h"

struct ElementSlot
{
	std::string name;
	bool isDiscovered;
	std::optional<std::string> imagePath;
};

class AlchemyViewModel
{
public:
	explicit AlchemyViewModel(AlchemyModel& m_alchemy)
		: m_alchemy(m_alchemy)
	{
	}

	std::vector<ElementSlot> ListElementsStatuses() const
	{
		std::unordered_set<ElementType> discoveredElements = m_alchemy.ListDiscoveredElements();
		const auto& allElements = RecipeBook::ListElements();

		std::vector<ElementSlot> result;
		for (ElementType el : allElements)
		{
			bool discovered = discoveredElements.contains(el);
			result.push_back({ ElementTypeName(el), discovered, std::nullopt });
		}
		return result;
	}

private:
	AlchemyModel& m_alchemy;
};