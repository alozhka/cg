#pragma once
#include "../viewModel/HangmanViewModel.hpp"
#include "AlphabetView.hpp"
#include "Drawer.hpp"
#include "HangmanView.h"

#include <SFML/Graphics/RenderWindow.hpp>

class GameView
{
public:
	GameView(sf::RenderWindow& window, const sf::Font& font, HangmanViewModel& hangmanViewModel)
		: m_window(window)
		, m_alphabetView(font, 50.0f, 450.0f, 30.0f, 10.0f)
		, m_hangmanView(m_window, font, hangmanViewModel)
		, m_hangmanViewModel(hangmanViewModel)
	{
	}

	void Run()
	{
		while (m_window.isOpen())
		{
			ProcessEvents();
			m_window.clear(sf::Color::White);
			Draw();
			m_window.display();
		}
	}

private:
	void Draw()
	{
		DrawGallows();
		DrawHangman();
		DrawWord();
		m_alphabetView.draw(m_window, m_hangmanViewModel.getAlphabetStates());
	}

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

	void ProcessEvents()
	{
		while (std::optional<sf::Event> event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				m_window.close();
			}
			if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
			{
				HandleResized(resizedEvent);
			}
			if (const auto* buttonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>())
			{
				HandleButtonPressed(buttonPressedEvent);
			}
		}
	}

	void HandleResized(const sf::Event::Resized* event)
	{
		sf::FloatRect visibleArea(
			sf::Vector2f(0, 0),
			{ static_cast<float>(event->size.x),
				static_cast<float>(event->size.y) });
		m_window.setView(sf::View(visibleArea));
	}

	void HandleButtonPressed(const sf::Event::MouseButtonPressed* event)
	{
		if (event->button == sf::Mouse::Button::Left)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
			sf::Vector2f worldPos = m_window.mapPixelToCoords(mousePos);
			HandleClick(worldPos.x, worldPos.y);
		}
	}

	void HandleClick(double x, double y)
	{
		std::optional<char> letter = m_alphabetView.getLetterAt(x, y);
		if (letter)
		{
			m_hangmanViewModel.processInput(*letter);
		}
	}

	sf::RenderWindow& m_window;
	sf::Font m_font;

	AlphabetView m_alphabetView;
	HangmanView m_hangmanView;
	HangmanViewModel& m_hangmanViewModel;
};