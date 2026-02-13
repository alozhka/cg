#pragma once
#include "shared/IObserver.h"

#include <SFML/Graphics/RenderWindow.hpp>

class GameView final : IObserver
{
public:
	explicit GameView(sf::RenderWindow& window)
		: m_window(window)
	{
	}

	void Run()
	{
		while (m_window.isOpen())
		{
			ProcessEvents();
			m_window.clear(sf::Color::White);
			// Do job
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
		}
	}

	void OnUpdate() override
	{
		// обновляем
	}

	sf::RenderWindow& m_window;
};