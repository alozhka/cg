#pragma once
#include "viewModel/HangmanViewModel.hpp"

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

class AlphabetView
{
public:
	AlphabetView(
		sf::RenderWindow& window,
		const sf::Font& font,
		HangmanViewModel& hangmanViewModel,
		float startX,
		float startY,
		float boxSize,
		float padding)
		: m_window(window)
		, m_font(font)
		, m_hangmanViewModel(hangmanViewModel)
		, m_startX(startX)
		, m_startY(startY)
		, m_boxSize(boxSize)
		, m_padding(padding)
	{
	}

	void Draw()
	{
		auto alphabetStates = m_hangmanViewModel.GetAlphabetStates();
		for (size_t i = 0; i < HangmanModel::ALPHABET.size(); ++i)
		{
			wchar_t c = HangmanModel::ALPHABET[i];
			float x = m_startX + (i % m_cols) * (m_boxSize + m_padding);
			float y = m_startY + (i / m_cols) * (m_boxSize + m_padding);

			bool isCorrect = (alphabetStates[i] == 1);
			bool isWrong = (alphabetStates[i] == 2);

			sf::RectangleShape box(sf::Vector2f(m_boxSize, m_boxSize));
			box.setPosition(sf::Vector2f(x, y));

			if (isCorrect)
				box.setFillColor(LIGHT_GREEN);
			else if (isWrong)
				box.setFillColor(LIGHT_RED);
			else
				box.setFillColor(GRAY);

			box.setOutlineColor(sf::Color::Black);
			box.setOutlineThickness(1.0f);

			m_window.draw(box);

			sf::Text text(m_font);
			text.setString(sf::String(c));
			text.setCharacterSize(static_cast<unsigned int>(m_boxSize * 0.6f));
			text.setFillColor(sf::Color::Black);

			sf::FloatRect bounds = text.getLocalBounds();

			float textCenterX = bounds.position.x + bounds.size.x / 2.0f;
			float textCenterY = bounds.position.y + bounds.size.y / 2.0f;

			float boxCenterX = x + m_boxSize / 2.0f;
			float boxCenterY = y + m_boxSize / 2.0f;

			text.setPosition(sf::Vector2f(boxCenterX - textCenterX, boxCenterY - textCenterY));

			m_window.draw(text);
		}
	}

	std::optional<wchar_t> GetLetterAt(double mouseX, double mouseY) const
	{
		for (size_t i = 0; i < HangmanModel::ALPHABET.size(); ++i)
		{
			float x = m_startX + (i % m_cols) * (m_boxSize + m_padding);
			float y = m_startY + (i / m_cols) * (m_boxSize + m_padding);

			if (mouseX >= x && mouseX <= x + m_boxSize && mouseY >= y && mouseY <= y + m_boxSize)
			{
				return HangmanModel::ALPHABET[i];
			}
		}
		return std::nullopt;
	}

private:
	HangmanViewModel& m_hangmanViewModel;
	int m_cols = 11;
	float m_startX, m_startY, m_boxSize, m_padding;

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	static constexpr sf::Color LIGHT_GREEN = sf::Color(204, 255, 204);
	static constexpr sf::Color LIGHT_RED = sf::Color(255, 204, 204);
	static constexpr sf::Color GRAY = sf::Color(230, 230, 230);
};
