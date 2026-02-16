#pragma once
#include "../model/AlchemyModel.h"
#include "../model/Element.h"
#include "../view/TextureCache.h"

#include <SFML/Graphics/Texture.hpp>

struct ElementSlot
{
	std::wstring name;
	bool isDiscovered;
	const sf::Texture* texture;
};

struct WorkspaceElementData
{
	std::string id;
	ElementSlot slot;
	sf::Vector2f position;
};

class AlchemyViewModel
{
public:
	AlchemyViewModel(AlchemyModel& alchemy, const std::string& assetsDir)
		: m_alchemy(alchemy)
		, m_textureCache(assetsDir)
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

	std::vector<ElementSlot> ListElementsStatuses()
	{
		std::unordered_set<ElementType> discoveredElements = m_alchemy.ListDiscoveredElements();
		const auto& allElements = m_alchemy.ListAllElements();

		std::vector<ElementSlot> result;
		for (ElementType el : allElements)
		{
			bool discovered = discoveredElements.contains(el);
			std::wstring name = discovered ? ElementTypeToString(el) : L"???";
			const sf::Texture* texture = discovered ? m_textureCache.Get(el) : nullptr;
			result.push_back({ name, discovered, texture });
		}
		return result;
	}

	std::vector<WorkspaceElementData> ListWorkspaceElements()
	{
		std::vector<Element> elements = m_alchemy.ListElementsOnWorkspace();
		std::vector<WorkspaceElementData> results;
		results.reserve(elements.size());

		for (const auto& element : elements)
		{
			const sf::Texture* texture = m_textureCache.Get(element.GetType());
			ElementSlot slot{ ElementTypeToString(element.GetType()), true, texture };
			results.push_back({ element.GetId(), slot, element.GetPosition() });
		}
		return results;
	}

private:
	AlchemyModel& m_alchemy;
	TextureCache m_textureCache;
};
