#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace Letters
{
constexpr static float LETTER_HEIGHT = 300.0f;

static void DrawLetterC(sf::RenderWindow& window, sf::Vector2f leftTop, sf::Color color)
{
	sf::RectangleShape leftRect(sf::Vector2f(50, 300));
	leftRect.setPosition(leftTop);
	leftRect.setFillColor(color);
	window.draw(leftRect);

	sf::RectangleShape aboveRect(sf::Vector2f(200, 50));
	aboveRect.setPosition(leftTop);
	aboveRect.setFillColor(color);
	window.draw(aboveRect);

	sf::RectangleShape lowerRect(sf::Vector2f(200, 50));
	lowerRect.setPosition(sf::Vector2f(leftTop.x, leftTop.y + 250));
	lowerRect.setFillColor(color);
	window.draw(lowerRect);
}

static void DrawLetterA(sf::RenderWindow& window, sf::Vector2f leftTop, sf::Color color)
{
	sf::RectangleShape leftRect(sf::Vector2f(50, LETTER_HEIGHT));
	leftRect.setPosition(leftTop);
	leftRect.setFillColor(color);
	window.draw(leftRect);

	sf::RectangleShape rightRect(sf::Vector2f(50, LETTER_HEIGHT));
	rightRect.setPosition(sf::Vector2f(leftTop.x + 150, leftTop.y));
	rightRect.setFillColor(color);
	window.draw(rightRect);

	sf::RectangleShape aboveRect(sf::Vector2f(150, 50));
	aboveRect.setPosition(leftTop);
	aboveRect.setFillColor(color);
	window.draw(aboveRect);

	sf::RectangleShape lowerRect(sf::Vector2f(150, 50));
	lowerRect.setPosition(sf::Vector2f(leftTop.x, leftTop.y + 150));
	lowerRect.setFillColor(color);
	window.draw(lowerRect);
}

static void DrawLetterL(sf::RenderWindow& window, sf::Vector2f leftTop, sf::Color color)
{
	sf::RectangleShape aboveRect(sf::Vector2f(150, 50));
	aboveRect.setPosition(sf::Vector2f(leftTop.x + 50, leftTop.y));
	aboveRect.setFillColor(color);
	window.draw(aboveRect);

	sf::RectangleShape leftRect(sf::Vector2f(50, LETTER_HEIGHT));
	leftRect.setPosition(sf::Vector2f(leftTop.x + 50, leftTop.y));
	leftRect.setFillColor(color);
	window.draw(leftRect);

	sf::RectangleShape lowerRect(sf::Vector2f(100, 50));
	lowerRect.setPosition(sf::Vector2f(leftTop.x, leftTop.y + 250));
	lowerRect.setFillColor(color);
	window.draw(lowerRect);

	sf::RectangleShape rightRect(sf::Vector2f(50, LETTER_HEIGHT));
	rightRect.setPosition(sf::Vector2f(leftTop.x + 150, leftTop.y));
	rightRect.setFillColor(color);
	window.draw(rightRect);
}
} // namespace Letters