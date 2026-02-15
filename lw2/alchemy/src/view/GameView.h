#pragma once
#include "shared/IObserver.h"
#include "view/DiscoveredPanel.h"
#include "view/WorkspaceArea.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class GameView final
{
public:
	explicit GameView(sf::RenderWindow& window, const sf::Font& font, AlchemyViewModel& alchemyViewModel)
		: m_window(window)
		, m_font(font)
		, m_discoveredPanel(
			  window,
			  font,
			  { { 0, 0 }, { PANEL_WIDTH, static_cast<float>(window.getSize().y) } },
			  alchemyViewModel)
		, m_workspaceArea(
			  window,
			  font,
			  { { PANEL_WIDTH, 0 }, { static_cast<float>(window.getSize().x) - PANEL_WIDTH, static_cast<float>(window.getSize().y) } },
			  alchemyViewModel)
	{
	}

	void Run()
	{
		while (m_window.isOpen())
		{
			ProcessEvents();
			m_window.clear(sf::Color::White);
			m_discoveredPanel.Draw();
			m_workspaceArea.Draw();
			m_window.display();
		}
	}

private:
	static constexpr float PANEL_WIDTH = 450;

	void ProcessEvents()
	{
		while (std::optional<sf::Event> event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				m_window.close();
			}
			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonReleased>())
			{
				HandleMouseReleased(mousePressed);
			}
		}
	}

	void HandleMouseReleased(const sf::Event::MouseButtonReleased* event)
	{
		if (event->button == sf::Mouse::Button::Left)
		{
			HandleClick(event->position);
		}
	}

	void HandleClick(sf::Vector2i pos)
	{
		m_discoveredPanel.HandleClick(pos);
	}

	sf::RenderWindow& m_window;
	const sf::Font& m_font;

	DiscoveredPanel m_discoveredPanel;
	WorkspaceArea m_workspaceArea;
};