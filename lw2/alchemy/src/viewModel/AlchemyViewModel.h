#pragma once
#include "../model/AlchemyModel.h"
#include "../model/Element.h"
#include "../view/TextureCache.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

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
		SetMessage(L"Добавлен: " + typeName);
	}

	void MoveElement(const std::string& id, sf::Vector2f newPos)
	{
		m_alchemy.MoveElement(id, newPos);
	}

	void RemoveElement(const std::string& id)
	{
		m_alchemy.RemoveElement(id);
		SetMessage(L"Элемент удалён");
	}

	void MergeElements(const std::string& sourceId, const std::string& targetId, sf::Vector2f dropPos)
	{
		size_t discoveredBefore = m_alchemy.ListDiscoveredElementsOrdered().size();
		bool elementCombined = m_alchemy.TryCombineElements(sourceId, targetId, dropPos);

		// TODO: вынести обработку действий в отдельный обработчик
		if (!elementCombined)
		{
			m_alchemy.MoveElement(sourceId, dropPos);
			SetMessage(L"Не удалось скомбинировать");
			return;
		}

		size_t discoveredAfter = m_alchemy.ListDiscoveredElementsOrdered().size();
		if (discoveredAfter > discoveredBefore)
		{
			const auto& discovered = m_alchemy.ListDiscoveredElementsOrdered();
			std::wstring newName = ElementTypeToString(discovered.back());
			SetMessage(L"Открыт новый элемент: " + newName);
		}
		else
		{
			SetMessage(L"Элементы скомбинированы");
		}
	}

	std::wstring GetMessage() const
	{
		if (m_lastMessage.empty())
		{
			return {};
		}
		if (m_messageClock.getElapsedTime().asSeconds() > MessageDuration)
		{
			return {};
		}
		return m_lastMessage;
	}

	float GetMessageAge() const
	{
		return m_messageClock.getElapsedTime().asSeconds();
	}

	void BringToFront(const std::string& id)
	{
		m_alchemy.BringToFront(id);
	}

	std::vector<ElementSlot> ListElementsStatuses()
	{
		const auto& discoveredOrder = m_alchemy.ListDiscoveredElementsOrdered();

		std::vector<ElementSlot> result;
		result.reserve(discoveredOrder.size());
		for (ElementType el : discoveredOrder)
		{
			std::wstring name = ElementTypeToString(el);
			const sf::Texture* texture = m_textureCache.Get(el);
			result.push_back({ name, true, texture });
		}
		return result;
	}

	void SortDiscoveredElements()
	{
		m_alchemy.SortDiscoveredElements();
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
	static constexpr float MessageDuration = 2.0f;

	void SetMessage(const std::wstring& message)
	{
		m_lastMessage = message;
		m_messageClock.restart();
	}

	AlchemyModel& m_alchemy;
	TextureCache m_textureCache;
	std::wstring m_lastMessage;
	sf::Clock m_messageClock;
};
