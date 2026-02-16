#pragma once

#include "../viewModel/AlchemyViewModel.h"
#include "Colors.h"
#include "ElementCardView.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class DiscoveredPanel
{
public:
	DiscoveredPanel(sf::RenderWindow& window, const sf::Font& font, sf::FloatRect rect,
		AlchemyViewModel& viewModel)
		: m_alchemyViewModel(viewModel)
		, m_window(window)
		, m_font(font)
		, m_rect(rect)
		, m_titleText(font, L"Открытые элементы", TitleFontSize)
	{
		SetupBackground();
		SetupTitle();
		RebuildCards();
	}

	void Draw()
	{
		RebuildCards();
		m_window.draw(m_background);
		m_window.draw(m_titleText);

		for (const auto& card : m_cards)
		{
			card.Draw(m_window);
		}
	}

	void RebuildCards()
	{
		m_cards.clear();

		auto elements = m_alchemyViewModel.ListElementsStatuses();
		int columns = CalculateColumnCount();
		sf::Vector2f gridOrigin = CalculateGridOrigin(columns);

		for (size_t i = 0; i < elements.size(); ++i)
		{
			sf::Vector2f position = GetCellPosition(gridOrigin, i, columns);
			const auto& colors = elements[i].isDiscovered ? Colors::DiscoveredCard : Colors::UndiscoveredCard;
			m_cards.emplace_back(m_font, elements[i], position, colors);
		}
	}

	void HandleClick(sf::Vector2f pos)
	{
		std::optional<std::wstring> newElement = GetPressedElementAt(pos);
		if (newElement.has_value())
		{
			m_alchemyViewModel.InsertElement(*newElement);
		}
	}

private:
	static constexpr unsigned int TitleFontSize = 20;
	static constexpr float TitleTopMargin = 10.f;
	static constexpr float GridTopMargin = 45.f;
	static constexpr float GridSidePadding = 10.f;
	static constexpr float GridGap = 8.f;

	std::optional<std::wstring> GetPressedElementAt(sf::Vector2f pos) const
	{
		for (const auto& card : m_cards)
		{
			const auto& slot = card.GetSlot();
			if (slot.isDiscovered && card.GetBounds().contains(pos))
			{
				return slot.name;
			}
		}
		return std::nullopt;
	}

	int CalculateColumnCount() const
	{
		float availableWidth = m_rect.size.x - GridSidePadding * 2.f;
		int columns = static_cast<int>((availableWidth + GridGap) / (ElementCardView::Width + GridGap));
		return std::max(columns, 1);
	}

	sf::Vector2f CalculateGridOrigin(int columns) const
	{
		float totalGridWidth = static_cast<float>(columns) * ElementCardView::Width
			+ static_cast<float>(columns - 1) * GridGap;

		return {
			m_rect.position.x + (m_rect.size.x - totalGridWidth) / 2.f,
			m_rect.position.y + GridTopMargin,
		};
	}

	static sf::Vector2f GetCellPosition(sf::Vector2f gridOrigin, size_t index, int columns)
	{
		int col = static_cast<int>(index) % columns;
		int row = static_cast<int>(index) / columns;

		return {
			gridOrigin.x + static_cast<float>(col) * (ElementCardView::Width + GridGap),
			gridOrigin.y + static_cast<float>(row) * (ElementCardView::Height + GridGap),
		};
	}

	void SetupBackground()
	{
		m_background.setPosition(m_rect.position);
		m_background.setSize(m_rect.size);
		m_background.setFillColor(Colors::LightGray);
	}

	void SetupTitle()
	{
		float textWidth = m_titleText.getLocalBounds().size.x;
		float centeredX = m_rect.position.x + (m_rect.size.x - textWidth) / 2.f;
		m_titleText.setPosition({ centeredX, m_rect.position.y + TitleTopMargin });
		m_titleText.setFillColor(sf::Color::Black);
		m_titleText.setStyle(sf::Text::Bold);
	}

	AlchemyViewModel& m_alchemyViewModel;
	std::vector<ElementCardView> m_cards;

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	sf::FloatRect m_rect;
	sf::RectangleShape m_background;
	sf::Text m_titleText;
};
