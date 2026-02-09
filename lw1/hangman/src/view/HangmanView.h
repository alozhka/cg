#pragma once
#include "../viewModel/HangmanViewModel.hpp"
#include "Drawer.hpp"

class HangmanView
{
public:
	HangmanView(
		sf::RenderWindow& m_window,
		const sf::Font& m_font,
		const HangmanViewModel& hangmanViewModel)
		: m_window(m_window)
		, m_font(m_font)
		, m_hangmanViewModel(hangmanViewModel)
	{
	}

	void Draw()
	{
		DrawGallows();
		DrawHangman();
		DrawWord();
	}

private:
	void DrawGallows()
	{
		Drawer::DrawLine(m_window, 50, 400, 150, 400, 3.0f);
		Drawer::DrawLine(m_window, 100, 400, 100, 100, 3.0f);
		Drawer::DrawLine(m_window, 100, 100, 250, 100, 3.0f);
		Drawer::DrawLine(m_window, 250, 100, 250, 150, 3.0f);
	}

	void DrawHangman()
	{
		auto wrongGuesses = m_hangmanViewModel.getWrongGuesses();
		float thickness = 2.0f;
		if (wrongGuesses >= 1) // Head
		{
			Drawer::drawCircle(m_window, 250, 170, 20, 20, thickness);
		}
		if (wrongGuesses >= 2) // Body
		{
			Drawer::DrawLine(m_window, 250, 190, 250, 270, thickness);
		}
		if (wrongGuesses >= 3) // Left Arm
		{
			Drawer::DrawLine(m_window, 250, 210, 220, 240, thickness);
		}
		if (wrongGuesses >= 4) // Right Arm
		{
			Drawer::DrawLine(m_window, 250, 210, 280, 240, thickness);
		}
		if (wrongGuesses >= 5) // Left Leg
		{
			Drawer::DrawLine(m_window, 250, 270, 230, 310, thickness);
		}
		if (wrongGuesses >= 6) // Right Leg
		{
			Drawer::DrawLine(m_window, 250, 270, 270, 310, thickness);
		}
	}

	void DrawWord() const
	{
		std::string word = m_hangmanViewModel.getDisplayWord();
		float startX = 400;
		float startY = 300;
		float charSize = 20;
		float spacing = 30;

		for (size_t i = 0; i < word.length(); ++i)
		{
			sf::Text text(m_font);
			text.setString(std::string(1, word[i]));
			text.setCharacterSize(static_cast<unsigned int>(charSize));
			text.setFillColor(sf::Color::Black);

			sf::FloatRect bounds = text.getLocalBounds();
			float textCenterX = bounds.position.x + bounds.size.x / 2.0f;
			float textCenterY = bounds.position.y + bounds.size.y / 2.0f;

			text.setPosition({ startX + i * spacing - textCenterX + charSize / 2.0f, startY - textCenterY + charSize / 2.0f });

			m_window.draw(text);
		}
	}

	sf::RenderWindow& m_window;
	sf::Font m_font;
	HangmanViewModel m_hangmanViewModel;
};