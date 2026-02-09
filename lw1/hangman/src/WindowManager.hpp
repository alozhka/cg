#pragma once
#include "HangmanViewModel.hpp"
#include "view/GameView.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>

class WindowManager
{
public:
	WindowManager(unsigned int width, unsigned int height, const std::string& title)
		: m_width(width)
		, m_height(height)
		, m_title(title)
		, m_viewModel("HANGMAN")
		, m_view(m_viewModel, m_font)
	{
	}

	bool init()
	{
		if (!m_font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf"))
		{
			std::cerr << "Failed to load font. Make sure a font is available at standard paths.\n";
			return false;
		}

		m_window = CreateWindow();
		if (!m_window.isOpen())
		{
			std::cerr << "Failed to create SFML window\n";
			return false;
		}

		m_window.setFramerateLimit(60);
		return true;
	}

	void run()
	{
		while (m_window.isOpen())
		{
			ProcessEvents();
			m_window.clear(sf::Color::White);
			m_view.draw(m_window);
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
				HandleResizedEvent(resizedEvent);
			}
			if (const auto* buttonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>())
			{
				HandleButtonPressed(buttonPressedEvent);
			}
		}
	}

	void HandleResizedEvent(const sf::Event::Resized* event)
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
			m_view.HandleClick(worldPos.x, worldPos.y);
		}
	}

	static sf::RenderWindow CreateWindow()
	{
		sf::ContextSettings settings;
		settings.antiAliasingLevel = 16;
		return sf::RenderWindow{
			sf::VideoMode(sf::Vector2u(800, 600)),
			"The Hangman Game",
			sf::Style::Default,
			sf::State::Windowed,
			settings
		};
	}

	sf::RenderWindow m_window;
	unsigned int m_width;
	unsigned int m_height;
	std::string m_title;

	sf::Font m_font;
	HangmanViewModel m_viewModel;
	GameView m_view;
};
