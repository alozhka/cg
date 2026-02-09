#include "AlphabetView.hpp"
#include <iostream>

AlphabetView::AlphabetView(const sf::Font& font, float startX, float startY, float boxSize, float padding)
	: m_font(font)
	, m_startX(startX)
	, m_startY(startY)
	, m_boxSize(boxSize)
	, m_padding(padding)
{
}

void AlphabetView::draw(sf::RenderTarget& target, const std::vector<int>& alphabetStates) const
{
	for (int i = 0; i < 26; ++i)
	{
		char c = 'A' + i;
		float x = m_startX + (i % m_cols) * (m_boxSize + m_padding);
		float y = m_startY + (i / m_cols) * (m_boxSize + m_padding);

		bool isCorrect = (alphabetStates[i] == 1);
		bool isWrong = (alphabetStates[i] == 2);

		sf::RectangleShape box(sf::Vector2f(m_boxSize, m_boxSize));
		box.setPosition(sf::Vector2f(x, y));

		if (isCorrect)
			box.setFillColor(sf::Color(204, 255, 204)); // Light green
		else if (isWrong)
			box.setFillColor(sf::Color(255, 204, 204)); // Light red
		else
			box.setFillColor(sf::Color(230, 230, 230)); // Gray

		box.setOutlineColor(sf::Color::Black);
		box.setOutlineThickness(1.0f);

		target.draw(box);

		// Draw char centered
		sf::Text text(m_font);
		text.setString(std::string(1, c));
		text.setCharacterSize(static_cast<unsigned int>(m_boxSize * 0.6f));
		text.setFillColor(sf::Color::Black);
		
		// Center text in box
		sf::FloatRect bounds = text.getLocalBounds();
		// In SFML 3, getLocalBounds() usually returns {left, top, width, height}.
		// To center: pos = boxCenter - textCenter.
		// boxCenter = x + boxSize/2, y + boxSize/2
		// textCenter = bounds.left + bounds.width/2, bounds.top + bounds.height/2
		
		float textCenterX = bounds.position.x + bounds.size.x / 2.0f;
		float textCenterY = bounds.position.y + bounds.size.y / 2.0f;
		
		float boxCenterX = x + m_boxSize / 2.0f;
		float boxCenterY = y + m_boxSize / 2.0f;

		text.setPosition(sf::Vector2f(boxCenterX - textCenterX, boxCenterY - textCenterY));

		target.draw(text);
	}
}

std::optional<char> AlphabetView::getLetterAt(double mouseX, double mouseY) const
{
	for (int i = 0; i < 26; ++i)
	{
		float x = m_startX + (i % m_cols) * (m_boxSize + m_padding);
		float y = m_startY + (i / m_cols) * (m_boxSize + m_padding);

		if (mouseX >= x && mouseX <= x + m_boxSize && mouseY >= y && mouseY <= y + m_boxSize)
		{
			return (char)('A' + i);
		}
	}
	return std::nullopt;
}
