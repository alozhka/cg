#pragma once

#include "../viewModel/AlchemyViewModel.h"
#include "Colors.h"
#include "ElementCardView.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <optional>
#include <ranges>

class WorkspaceArea
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
	}

	void Draw()
	{
		m_window.draw(m_background);
		m_window.draw(m_titleText);
		DrawElements();
	}

	void HandleMousePressed(sf::Vector2f pos)
	{
		if (!m_rect.contains(pos))
		{
			return;
		}

		std::optional<std::string> elementId = FindElementAt(pos);
		if (!elementId.has_value())
		{
			return;
		}

		sf::Vector2f elementPos = FindElementPosition(*elementId);
		m_dragState = DragState{
			.elementId = *elementId,
			.offset = pos - PositionToAbsolute(elementPos),
			.currentRelativePos = elementPos,
		};
	}

	void HandleMouseMoved(sf::Vector2f mousePos)
	{
		if (!IsDragging())
		{
			return;
		}

		sf::Vector2f newAbsPos = mousePos - m_dragState->offset;
		sf::Vector2f clampedAbsPos = ClampToWorkspace(newAbsPos);

		m_dragState->currentRelativePos = AbsoluteToPosition(clampedAbsPos);
	}

	void HandleMouseReleased()
	{
		if (!IsDragging())
		{
			return;
		}

		m_viewModel.MoveElement(m_dragState->elementId, m_dragState->currentRelativePos);
		m_dragState.reset();
	}

	bool IsDragging() const
	{
		return m_dragState.has_value();
	}

private:
	struct DragState
	{
		std::string elementId;
		sf::Vector2f offset;
		sf::Vector2f currentRelativePos;
	};

	std::optional<std::string> FindElementAt(sf::Vector2f absolutePos) const
	{
		auto items = m_viewModel.ListWorkspaceElements();

		// Перебираем с конца — верхний элемент первый
		for (auto& item : std::ranges::reverse_view(items))
		{
			sf::Vector2f absItemPos = PositionToAbsolute(item.position);
			sf::FloatRect bounds{ absItemPos, { ElementCardView::Width, ElementCardView::Height } };

			if (bounds.contains(absolutePos))
			{
				return item.id;
			}
		}
		return std::nullopt;
	}

	sf::Vector2f FindElementPosition(const std::string& id) const
	{
		auto items = m_viewModel.ListWorkspaceElements();
		for (const auto& item : items)
		{
			if (item.id == id)
				return item.position;
		}
		return {};
	}

	void DrawElements()
	{
		auto items = m_viewModel.ListWorkspaceElements();
		for (const auto& item : items)
		{
			sf::Vector2f drawPos = item.position;

			if (m_dragState.has_value() && m_dragState->elementId == item.id)
			{
				drawPos = m_dragState->currentRelativePos;
			}

			ElementSlot slot{ item.name, true, std::nullopt };
			ElementCardView card(m_font, slot, PositionToAbsolute(drawPos));
			card.Draw(m_window);
		}
	}

	sf::Vector2f ClampToWorkspace(sf::Vector2f absPos) const
	{
		float minX = m_rect.position.x;
		float minY = m_rect.position.y;
		float maxX = m_rect.position.x + m_rect.size.x - ElementCardView::Width;
		float maxY = m_rect.position.y + m_rect.size.y - ElementCardView::Height;

		return {
			std::clamp(absPos.x, minX, maxX),
			std::clamp(absPos.y, minY, maxY),
		};
	}

	sf::Vector2f PositionToAbsolute(sf::Vector2f pos) const
	{
		return { pos.x + m_rect.position.x, pos.y + m_rect.position.y };
	}

	sf::Vector2f AbsoluteToPosition(sf::Vector2f absPos) const
	{
		return { absPos.x - m_rect.position.x, absPos.y - m_rect.position.y };
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

	std::optional<DragState> m_dragState;
};
