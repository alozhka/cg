#pragma once

#include "AlchemyViewModel.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <optional>

class ElementCard
{
public:
	static constexpr float Width = 100.f;
	static constexpr float Height = 120.f;
	static constexpr float ImageSize = 64.f;
	static constexpr float ImageTopMargin = 10.f;
	static constexpr float TextTopMargin = 80.f;
	static constexpr unsigned int FontSize = 12;

	ElementCard(const sf::Font& font, const ElementSlot& slot, sf::Vector2f position)
		: m_font(font)
		, m_slot(slot)
		, m_position(position)
	{
		LoadTexture();
	}

	void Draw(sf::RenderWindow& window) const
	{
		sf::RectangleShape card({ Width, Height });
		card.setPosition(m_position);

		if (m_slot.isDiscovered)
		{
			card.setFillColor(sf::Color(220, 235, 255));
			card.setOutlineColor(sf::Color(100, 150, 220));
		}
		else
		{
			card.setFillColor(sf::Color(215, 215, 215));
			card.setOutlineColor(sf::Color(180, 180, 180));
		}
		card.setOutlineThickness(1.f);
		window.draw(card);

		DrawImage(window);
		DrawLabel(window);
	}

	sf::FloatRect GetBounds() const
	{
		return { m_position, { Width, Height } };
	}

	const ElementSlot& GetSlot() const { return m_slot; }

private:
	void LoadTexture()
	{
		if (!m_slot.imagePath.has_value())
			return;

		m_texture.emplace(m_slot.imagePath.value());
		m_texture->setSmooth(true);
	}

	void DrawImage(sf::RenderWindow& window) const
	{
		float imageX = m_position.x + (Width - ImageSize) / 2.f;
		float imageY = m_position.y + ImageTopMargin;

		if (m_texture.has_value())
		{
			sf::Sprite sprite(*m_texture);
			auto texSize = m_texture->getSize();
			float scaleX = ImageSize / static_cast<float>(texSize.x);
			float scaleY = ImageSize / static_cast<float>(texSize.y);
			sprite.setScale({ scaleX, scaleY });
			sprite.setPosition({ imageX, imageY });
			window.draw(sprite);
		}
		else
		{
			// Плейсхолдер — серый квадрат с "?"
			sf::RectangleShape placeholder({ ImageSize, ImageSize });
			placeholder.setPosition({ imageX, imageY });
			placeholder.setFillColor(
				m_slot.isDiscovered ? sf::Color(200, 215, 240) : sf::Color(195, 195, 195));
			placeholder.setOutlineColor(
				m_slot.isDiscovered ? sf::Color(140, 170, 210) : sf::Color(165, 165, 165));
			placeholder.setOutlineThickness(1.f);
			window.draw(placeholder);

			sf::Text questionMark(m_font, "?", 28);
			questionMark.setFillColor(
				m_slot.isDiscovered ? sf::Color(100, 140, 200) : sf::Color(150, 150, 150));
			auto bounds = questionMark.getLocalBounds();
			questionMark.setPosition({
				imageX + (ImageSize - bounds.size.x) / 2.f,
				imageY + (ImageSize - bounds.size.y) / 2.f - 4.f,
			});
			window.draw(questionMark);
		}
	}

	void DrawLabel(sf::RenderWindow& window) const
	{
		sf::Text label(m_font, "", FontSize);

		if (m_slot.isDiscovered)
		{
			label.setString(sf::String::fromUtf8(m_slot.name.begin(), m_slot.name.end()));
			label.setFillColor(sf::Color(30, 30, 30));
		}
		else
		{
			label.setString("???");
			label.setFillColor(sf::Color(140, 140, 140));
		}

		auto textBounds = label.getLocalBounds();
		float textX = m_position.x + (Width - textBounds.size.x) / 2.f;
		float textY = m_position.y + TextTopMargin;
		label.setPosition({ textX, textY });
		window.draw(label);
	}

	const sf::Font& m_font;
	ElementSlot m_slot;
	sf::Vector2f m_position;
	std::optional<sf::Texture> m_texture;
};
