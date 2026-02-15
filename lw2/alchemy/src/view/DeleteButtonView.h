#pragma once

#include "Colors.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class DeleteButtonView
{
public:
	static constexpr float Size = 60.f;
	static constexpr float Margin = 20.f;

	DeleteButtonView(const sf::Font& font, sf::FloatRect workspaceRect)
		: m_font(font)
		, m_bounds(CalculateBounds(workspaceRect))
	{
	}

	void Draw(sf::RenderWindow& window, bool hovered) const
	{
		const auto& colors = hovered
			? Colors::DeleteButtonHovered
			: Colors::DeleteButtonNormal;

		DrawBackground(window, colors);
		DrawCross(window, colors);
	}

	bool Contains(sf::Vector2f point) const
	{
		sf::Vector2f center = GetCenter();
		float radius = Size / 2.f;
		sf::Vector2f diff = point - center;
		return (diff.x * diff.x + diff.y * diff.y) <= (radius * radius);
	}

	sf::FloatRect GetBounds() const
	{
		return m_bounds;
	}

private:
	static constexpr float OutlineThickness = 2.f;
	static constexpr float CrossArmLength = 12.f;
	static constexpr float CrossThickness = 3.f;

	static sf::FloatRect CalculateBounds(sf::FloatRect workspaceRect)
	{
		float x = workspaceRect.position.x + workspaceRect.size.x - Size - Margin;
		float y = workspaceRect.position.y + workspaceRect.size.y - Size - Margin;
		return { { x, y }, { Size, Size } };
	}

	sf::Vector2f GetCenter() const
	{
		return {
			m_bounds.position.x + Size / 2.f,
			m_bounds.position.y + Size / 2.f,
		};
	}

	void DrawBackground(sf::RenderWindow& window, const Colors::DeleteButtonColorScheme& colors) const
	{
		sf::CircleShape circle(Size / 2.f);
		circle.setPosition(m_bounds.position);
		circle.setFillColor(colors.Fill);
		circle.setOutlineColor(colors.Outline);
		circle.setOutlineThickness(OutlineThickness);
		window.draw(circle);
	}

	void DrawCross(sf::RenderWindow& window, const Colors::DeleteButtonColorScheme& colors) const
	{
		sf::Vector2f center = GetCenter();

		sf::RectangleShape horizontal({ CrossArmLength * 2.f, CrossThickness });
		horizontal.setOrigin({ CrossArmLength, CrossThickness / 2.f });
		horizontal.setPosition(center);
		horizontal.setRotation(sf::degrees(45.f));
		horizontal.setFillColor(colors.Icon);
		window.draw(horizontal);

		sf::RectangleShape vertical({ CrossArmLength * 2.f, CrossThickness });
		vertical.setOrigin({ CrossArmLength, CrossThickness / 2.f });
		vertical.setPosition(center);
		vertical.setRotation(sf::degrees(-45.f));
		vertical.setFillColor(colors.Icon);
		window.draw(vertical);
	}

	const sf::Font& m_font;
	sf::FloatRect m_bounds;
};
