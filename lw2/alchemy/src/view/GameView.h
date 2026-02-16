#pragma once
#include "shared/IObserver.h"
#include "view/DiscoveredPanel.h"
#include "view/TextureCache.h"
#include "view/WorkspaceArea.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class GameView final : IObserver
{
public:
	explicit GameView(
		sf::RenderWindow& window,
		const sf::Font& font,
		AlchemyViewModel& alchemyViewModel,
		const std::string& assetsDir)
		: m_window(window)
		, m_font(font)
		, m_iconTextureCache(assetsDir)
		, m_discoveredPanel(
			  window,
			  font,
			  { { 0, 0 }, { PANEL_WIDTH, static_cast<float>(window.getSize().y) } },
			  alchemyViewModel,
			  m_iconTextureCache)
		, m_workspaceArea(
			  window,
			  font,
			  { { PANEL_WIDTH, 0 }, { static_cast<float>(window.getSize().x) - PANEL_WIDTH, static_cast<float>(window.getSize().y) } },
			  alchemyViewModel,
			  m_iconTextureCache)
		, m_alchemyViewModel(alchemyViewModel)
	{
		m_alchemyViewModel.AddObserver(this);
	}

	~GameView() override
	{
		m_alchemyViewModel.RemoveObserver(this);
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
		m_discoveredPanel.Draw();
		m_workspaceArea.Draw();
	}

	void ProcessEvents()
	{
		while (std::optional<sf::Event> event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				HandleWindowClosed();
				break;
			}
			if (const auto* pressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				HandleMousePressed(pressed);
			}
			if (const auto* moved = event->getIf<sf::Event::MouseMoved>())
			{
				HandleMouseMoved(moved);
			}
			if (const auto* released = event->getIf<sf::Event::MouseButtonReleased>())
			{
				HandleMouseReleased(released);
			}
		}
	}

	void HandleWindowClosed()
	{
		m_window.close();
	}

	void HandleMousePressed(const sf::Event::MouseButtonPressed* pressed)
	{
		if (pressed->button == sf::Mouse::Button::Left)
		{
			sf::Vector2f pos = ToFloatVector(pressed->position);
			m_workspaceArea.HandleMousePressed(pos);
		}
	}

	void HandleMouseMoved(const sf::Event::MouseMoved* moved)
	{
		sf::Vector2f pos = ToFloatVector(moved->position);
		m_workspaceArea.HandleMouseMoved(pos);
	}

	void HandleMouseReleased(const sf::Event::MouseButtonReleased* released)
	{
		if (released->button == sf::Mouse::Button::Left)
		{
			sf::Vector2f pos = ToFloatVector(released->position);
			m_workspaceArea.HandleMouseReleased();
			m_discoveredPanel.HandleClick(pos);
		}
	}

	static sf::Vector2f ToFloatVector(sf::Vector2i intVector)
	{
		return {
			static_cast<float>(intVector.x),
			static_cast<float>(intVector.y)
		};
	}

	void OnUpdate() override
	{
		Draw();
	}

	static constexpr float PANEL_WIDTH = 450;

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	TextureCache m_iconTextureCache;

	DiscoveredPanel m_discoveredPanel;
	WorkspaceArea m_workspaceArea;

	AlchemyViewModel& m_alchemyViewModel;
};
