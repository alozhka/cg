#pragma once
#include "../viewModel/HangmanViewModel.hpp"
#include "AlphabetView.h"
#include "Drawer.h"
#include "HangmanView.h"

#include <SFML/Graphics/RenderWindow.hpp>

class GameView
{
public:
	GameView(sf::RenderWindow& window, const sf::Font& font, HangmanViewModel& hangmanViewModel)
		: m_window(window)
		, m_alphabetView(window, font, hangmanViewModel, 50.0f, 450.0f, 30.0f, 10.0f)
		, m_hangmanView(window, font, hangmanViewModel)
		, m_hangmanViewModel(hangmanViewModel)
	{
	}

	void Run()
	{
		while (m_window.isOpen())
		{
			ProcessEvents();
			m_window.clear(sf::Color::White);
			m_hangmanView.Draw();
			m_alphabetView.Draw();
			m_window.display();
		}
	}

private:
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
		std::optional<wchar_t> letter = m_alphabetView.GetLetterAt(x, y);
		if (letter)
		{
			m_hangmanViewModel.ProcessInput(*letter);
		}
	}

	sf::RenderWindow& m_window;
	sf::Font m_font;

	AlphabetView m_alphabetView;
	HangmanView m_hangmanView;
	HangmanViewModel& m_hangmanViewModel;
};