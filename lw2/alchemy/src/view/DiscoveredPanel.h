#pragma once

#include "Colors.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class DiscoveredPanel
{
public:
	DiscoveredPanel(sf::RenderWindow& window, const sf::Font& font, sf::FloatRect rect)
		: m_window(window)
		, m_font(font)
		, m_rect(rect)
		, m_titleText(font, L"Элементы", 20)
	{
		InitView();
	}

	void Draw()
	{
		m_window.draw(m_background);
		m_window.draw(m_titleText);
	}

private:
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

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	sf::FloatRect m_rect;

	sf::RectangleShape m_background;
	sf::Text m_titleText;
};
