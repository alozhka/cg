#pragma once

#include "AlchemyViewModel.h"
#include "Colors.h"
#include "view/ElementCard.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class DiscoveredPanel
{
public:
	DiscoveredPanel(sf::RenderWindow& window, const sf::Font& font, sf::FloatRect rect, AlchemyViewModel& alchemyViewModel)
		: m_alchemyViewModel(alchemyViewModel)
		, m_window(window)
		, m_font(font)
		, m_rect(rect)
		, m_titleText(font, L"Элементы", 20)
	{
		InitView();
		InitCards();
	}

	void Draw()
	{
		m_window.draw(m_background);
		m_window.draw(m_titleText);

		for (const auto& card : m_cards)
		{
			card.Draw(m_window);
		}
	}

	void Rebuild()
	{
		InitCards();
	}

private:
	static constexpr float GridPadding = 10.f;
	static constexpr float GridGap = 8.f;
	static constexpr float GridStartY = 45.f;

	void InitCards()
	{
		m_cards.clear();
		std::vector<ElementSlot> elements = m_alchemyViewModel.ListElementsStatuses();

		float availableWidth = m_rect.size.x - GridPadding * 2;
		int columns = static_cast<int>((availableWidth + GridGap) / (ElementCard::Width + GridGap));
		if (columns < 1)
			columns = 1;

		float totalGridWidth = columns * ElementCard::Width + (columns - 1) * GridGap;
		float offsetX = m_rect.position.x + (m_rect.size.x - totalGridWidth) / 2.f;
		float offsetY = m_rect.position.y + GridStartY;

		for (size_t i = 0; i < elements.size(); ++i)
		{
			int col = static_cast<int>(i) % columns;
			int row = static_cast<int>(i) / columns;

			float x = offsetX + col * (ElementCard::Width + GridGap);
			float y = offsetY + row * (ElementCard::Height + GridGap);

			m_cards.emplace_back(m_font, elements[i], sf::Vector2f{ x, y });
		}
	}

	void InitView()
	{
		m_background.setPosition(m_rect.position);
		m_background.setSize(m_rect.size);
		m_background.setFillColor(Colors::LightGray);

		float textWidth = m_titleText.getLocalBounds().size.x;
		float centeredX = m_rect.position.x + (m_rect.size.x - textWidth) / 2;
		m_titleText.setPosition({ centeredX, m_rect.position.y + 10 });
		m_titleText.setFillColor(sf::Color::Black);
		m_titleText.setStyle(sf::Text::Bold);
	}

	AlchemyViewModel& m_alchemyViewModel;
	std::vector<ElementCard> m_cards;

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	sf::FloatRect m_rect;
	sf::RectangleShape m_background;
	sf::Text m_titleText;
};
