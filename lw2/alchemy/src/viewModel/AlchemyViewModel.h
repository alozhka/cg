#pragma once
#include "../model/AlchemyModel.h"
#include "../model/Element.h"

struct ElementSlot
{
	std::wstring name;
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

	void AddObserver(IObserver* observer)
	{
		m_alchemy.AddObserver(observer);
	}

	void RemoveObserver(IObserver* observer)
	{
		m_alchemy.RemoveObserver(observer);
	}

	void InsertElement(const std::wstring& typeName)
	{
		ElementType elementType = ConvertToElementType(typeName);
		m_alchemy.InsertElement(elementType);
	}

	void MoveElement(const std::string& id, sf::Vector2f newPos)
	{
		m_alchemy.MoveElement(id, newPos);
	}

	void RemoveElement(const std::string& id)
	{
		m_alchemy.RemoveElement(id);
	}

	void MergeElements(const std::string& sourceId, const std::string& targetId, sf::Vector2f dropPos)
	{
		bool elementCombined = m_alchemy.TryCombineElements(sourceId, targetId, dropPos);
		if (!elementCombined)
		{
			m_alchemy.MoveElement(sourceId, dropPos);
		}
	}

	void BringToFront(const std::string& id)
	{
		m_alchemy.BringToFront(id);
	}

	std::vector<ElementSlot> ListElementsStatuses() const
	{
		std::unordered_set<ElementType> discoveredElements = m_alchemy.ListDiscoveredElements();
		const auto& allElements = m_alchemy.ListAllElements();

		std::vector<ElementSlot> result;
		for (ElementType el : allElements)
		{
			bool discovered = discoveredElements.contains(el);
			result.push_back({ ElementTypeToString(el), discovered, std::nullopt });
		}
		return result;
	}

	struct WorkspaceElementData
	{
		std::string id;
		std::wstring name;
		sf::Vector2f position;
	};

	std::vector<WorkspaceElementData> ListWorkspaceElements() const
	{
		std::vector<Element> elements = m_alchemy.ListElementsOnWorkspace();
		std::vector<WorkspaceElementData> results;
		results.reserve(elements.size());

		for (const auto& element : elements)
		{
			results.emplace_back(
				element.GetId(),
				ElementTypeToString(element.GetType()),
				element.GetPosition());
		}
		return results;
	}

private:
	AlchemyModel& m_alchemy;
};