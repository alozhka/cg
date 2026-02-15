#pragma once

#include "AlchemyViewModel.h"
#include "Colors.h"
#include "ElementCardView.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class WorkspaceArea : public IObserver
{
public:
	WorkspaceArea(sf::RenderWindow& window, const sf::Font& font, sf::FloatRect rect, AlchemyViewModel& viewModel)
		: m_window(window)
		, m_font(font)
		, m_rect(rect)
		, m_viewModel(viewModel)
		, m_titleText(font, L"Поле для экспериментов", TitleFontSize)
	{
		SetupBackground();
		SetupTitle();
		m_viewModel.AddObserver(this);
	}

	void Draw()
	{
		m_window.draw(m_background);
		m_window.draw(m_titleText);
		DrawElements();
	}

private:
	void OnUpdate() override
	{
		Draw();
	}

	void DrawElements()
	{
		auto items = m_viewModel.ListWorkspaceElements();
		for (const auto& item : items)
		{
			ElementSlot slot{ item.name, true, std::nullopt };
			ElementCardView card(m_font, slot, PositionToAbsolute(item.position));
			card.Draw(m_window);
		}
	}

	sf::Vector2f PositionToAbsolute(sf::Vector2f pos) const
	{
		return { pos.x + m_rect.position.x, pos.y + m_rect.position.y };
	}

	void SetupBackground()
	{
		m_background.setPosition(m_rect.position);
		m_background.setSize(m_rect.size);
		m_background.setFillColor(sf::Color::White);
	}

	void SetupTitle()
	{
		float textWidth = m_titleText.getLocalBounds().size.x;
		float centeredX = m_rect.position.x + (m_rect.size.x - textWidth) / 2.f;
		m_titleText.setPosition({ centeredX, m_rect.position.y + TitleTopMargin });
		m_titleText.setFillColor(sf::Color(120, 120, 120));
		m_titleText.setStyle(sf::Text::Bold);
	}

	static constexpr unsigned int TitleFontSize = 20;
	static constexpr float TitleTopMargin = 10.f;

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	sf::FloatRect m_rect;
	AlchemyViewModel& m_viewModel;

	sf::RectangleShape m_background;
	sf::Text m_titleText;
};
