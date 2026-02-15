#pragma once

#include "../viewModel/AlchemyViewModel.h"
#include "Colors.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <optional>

class ElementCardView
{
public:
	static constexpr float Width = 100.f;
	static constexpr float Height = 120.f;
	static constexpr float ImageSize = 64.f;
	static constexpr float ImageTopMargin = 10.f;
	static constexpr float LabelTopMargin = 80.f;
	static constexpr float OutlineThickness = 1.f;
	static constexpr unsigned int LabelFontSize = 12;
	static constexpr unsigned int PlaceholderFontSize = 28;

	ElementCardView(const sf::Font& font, const ElementSlot& slot, sf::Vector2f position)
		: m_font(font)
		, m_slot(slot)
		, m_position(position)
		, m_colors(slot.isDiscovered ? Colors::DiscoveredCard : Colors::UndiscoveredCard)
	{
		if (m_slot.imagePath.has_value())
		{
			m_texture.emplace(m_slot.imagePath.value());
			m_texture->setSmooth(true);
		}
	}

	void Draw(sf::RenderWindow& window) const
	{
		DrawBackground(window);
		DrawImage(window);
		DrawLabel(window);
	}

	sf::FloatRect GetBounds() const
	{
		return { m_position, { Width, Height } };
	}

	const ElementSlot& GetSlot() const { return m_slot; }

private:
	void DrawBackground(sf::RenderWindow& window) const
	{
		sf::RectangleShape card({ Width, Height });
		card.setPosition(m_position);
		card.setFillColor(m_colors.CardFill);
		card.setOutlineColor(m_colors.CardOutline);
		card.setOutlineThickness(OutlineThickness);
		window.draw(card);
	}

	void DrawImage(sf::RenderWindow& window) const
	{
		sf::Vector2f imagePos = GetImagePosition();

		if (m_texture.has_value())
		{
			DrawTextureSprite(window, imagePos);
		}
		else
		{
			DrawPlaceholder(window, imagePos);
		}
	}

	void DrawTextureSprite(sf::RenderWindow& window, sf::Vector2f imagePos) const
	{
		sf::Sprite sprite(*m_texture);
		auto texSize = m_texture->getSize();
		sprite.setScale({
			ImageSize / static_cast<float>(texSize.x),
			ImageSize / static_cast<float>(texSize.y),
		});
		sprite.setPosition(imagePos);
		window.draw(sprite);
	}

	void DrawPlaceholder(sf::RenderWindow& window, sf::Vector2f imagePos) const
	{
		sf::RectangleShape placeholder({ ImageSize, ImageSize });
		placeholder.setPosition(imagePos);
		placeholder.setFillColor(m_colors.PlaceholderFill);
		placeholder.setOutlineColor(m_colors.PlaceholderOutline);
		placeholder.setOutlineThickness(OutlineThickness);
		window.draw(placeholder);

		sf::Text symbol(m_font, "?", PlaceholderFontSize);
		symbol.setFillColor(m_colors.PlaceholderText);
		auto bounds = symbol.getLocalBounds();
		symbol.setPosition({
			imagePos.x + (ImageSize - bounds.size.x) / 2.f,
			imagePos.y + (ImageSize - bounds.size.y) / 2.f - 4.f,
		});
		window.draw(symbol);
	}

	void DrawLabel(sf::RenderWindow& window) const
	{
		sf::Text label(m_font, m_slot.name, LabelFontSize);
		label.setFillColor(m_colors.LabelText);

		auto textBounds = label.getLocalBounds();
		label.setPosition({
			m_position.x + (Width - textBounds.size.x) / 2.f,
			m_position.y + LabelTopMargin,
		});
		window.draw(label);
	}

	sf::Vector2f GetImagePosition() const
	{
		return {
			m_position.x + (Width - ImageSize) / 2.f,
			m_position.y + ImageTopMargin,
		};
	}

	const sf::Font& m_font;
	ElementSlot m_slot;
	sf::Vector2f m_position;
	Colors::CardColorScheme m_colors;
	std::optional<sf::Texture> m_texture;
};
