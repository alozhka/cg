#pragma once

#include "../viewModel/AlchemyViewModel.h"
#include "Colors.h"
#include "DeleteButtonView.h"
#include "ElementCardView.h"
#include "TextureCache.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <optional>
#include <ranges>

class WorkspaceArea
{
public:
	WorkspaceArea(
		sf::RenderWindow& window,
		const sf::Font& font,
		sf::FloatRect rect,
		AlchemyViewModel& viewModel,
		TextureCache& textureCache)
		: m_viewModel(viewModel)
		, m_textureCache(textureCache)
		, m_window(window)
		, m_font(font)
		, m_rect(rect)
		, m_titleText(font, L"Поле для экспериментов", TitleFontSize)
		, m_deleteButton(font, rect)
	{
		SetupBackground();
		SetupTitle();
	}

	void Draw()
	{
		m_window.draw(m_background);
		m_window.draw(m_titleText);
		DrawElements();
		DrawDeleteButton();
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

		m_viewModel.BringToFront(*elementId);

		sf::Vector2f elementPos = FindElementPosition(*elementId);
		m_dragState = DragState{
			.elementId = *elementId,
			.offset = pos - LocalPositionToAbsolute(elementPos),
			.currentMousePos = pos,
		};
	}

	void HandleMouseMoved(sf::Vector2f mousePos)
	{
		if (!IsDragging())
		{
			return;
		}

		m_dragState->currentMousePos = mousePos;
	}

	void HandleMouseReleased()
	{
		if (!IsDragging())
		{
			return;
		}

		if (IsCursorOverDeleteButton())
		{
			m_viewModel.RemoveElement(m_dragState->elementId);
		}
		else if (std::optional<std::string> targetId = FindDropTargetElement())
		{
			m_viewModel.MergeElements(m_dragState->elementId, *targetId, GetDraggedElementPosition());
		}
		else
		{
			m_viewModel.MoveElement(m_dragState->elementId, GetDraggedElementPosition());
		}
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
		sf::Vector2f currentMousePos;
	};

	sf::Vector2f GetDraggedElementPosition() const
	{
		sf::Vector2f absPos = ClampToWorkspace(m_dragState->currentMousePos - m_dragState->offset);
		return AbsolutePositionToLocal(absPos);
	}

	bool IsCursorOverDeleteButton() const
	{
		return m_deleteButton.Contains(m_dragState->currentMousePos);
	}

	std::optional<std::string> FindElementAt(sf::Vector2f absolutePos, const std::string& excludeId = {}) const
	{
		auto items = m_viewModel.ListWorkspaceElements();

		// Перебираем с конца — верхний элемент первый
		for (auto& item : std::ranges::reverse_view(items))
		{
			if (item.id == excludeId)
			{
				continue;
			}

			sf::Vector2f absItemPos = LocalPositionToAbsolute(item.position);
			sf::FloatRect bounds{ absItemPos, { ElementCardView::Width, ElementCardView::Height } };

			if (bounds.contains(absolutePos))
			{
				return item.id;
			}
		}
		return std::nullopt;
	}

	std::optional<std::string> FindDropTargetElement() const
	{
		return FindElementAt(m_dragState->currentMousePos, m_dragState->elementId);
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
		std::optional<std::string> dropTargetId = IsDragging() ? FindDropTargetElement() : std::nullopt;

		for (auto& item : items)
		{
			item.slot.texture = m_textureCache.Get(item.slot.type);
			sf::Vector2f drawPos = GetElementDrawPosition(item);
			Colors::CardColorScheme colors = GetElementColorScheme(item, dropTargetId);

			ElementCardView card(m_font, item.slot, LocalPositionToAbsolute(drawPos), colors);
			card.Draw(m_window);
		}
	}

	sf::Vector2f GetElementDrawPosition(const AlchemyViewModel::WorkspaceElementData& item) const
	{
		if (m_dragState.has_value() && m_dragState->elementId == item.id)
		{
			return GetDraggedElementPosition();
		}
		return item.position;
	}

	Colors::CardColorScheme GetElementColorScheme(
		const AlchemyViewModel::WorkspaceElementData& item,
		const std::optional<std::string>& dropTargetId) const
	{
		if (m_dragState.has_value() && m_dragState->elementId == item.id)
		{
			return Colors::DraggedCard;
		}
		if (dropTargetId.has_value() && *dropTargetId == item.id)
		{
			return Colors::DropTargetCard;
		}
		return Colors::DiscoveredCard;
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

	sf::Vector2f LocalPositionToAbsolute(sf::Vector2f pos) const
	{
		return { pos.x + m_rect.position.x, pos.y + m_rect.position.y };
	}

	sf::Vector2f AbsolutePositionToLocal(sf::Vector2f absPos) const
	{
		return { absPos.x - m_rect.position.x, absPos.y - m_rect.position.y };
	}

	void DrawDeleteButton()
	{
		if (IsDragging())
		{
			bool hovered = IsCursorOverDeleteButton();
			m_deleteButton.Draw(m_window, hovered);
		}
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

	AlchemyViewModel& m_viewModel;
	TextureCache& m_textureCache;

	sf::RenderWindow& m_window;
	const sf::Font& m_font;
	sf::FloatRect m_rect;
	sf::RectangleShape m_background;
	sf::Text m_titleText;
	DeleteButtonView m_deleteButton;
	std::optional<DragState> m_dragState;
};
