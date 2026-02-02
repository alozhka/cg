#include "SFML/Graphics/RectangleShape.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

sf::RenderWindow CreateWindow();
void DrawInitials(sf::RenderWindow& window);

int main()
{
	sf::RenderWindow window = CreateWindow();
	window.clear(sf::Color::White);

	DrawInitials(window);

	while (window.isOpen())
	{
		while (const std::optional<sf::Event> event = window.pollEvent())
		{
			if (event.has_value() && event->is<sf::Event::Closed>())
			{
				window.close();
				break;
			}
		}
	}

	return 0;
}

void DrawLetterC(sf::RenderWindow& window, sf::Vector2f leftTop, sf::Color color)
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

void DrawLetterA(sf::RenderWindow& window, sf::Vector2f leftTop, sf::Color color)
{
	sf::RectangleShape leftRect(sf::Vector2f(50, 300));
	leftRect.setPosition(leftTop);
	leftRect.setFillColor(color);
	window.draw(leftRect);

	sf::RectangleShape rightRect(sf::Vector2f(50, 300));
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

void DrawLetterL(sf::RenderWindow& window, sf::Vector2f leftTop, sf::Color color)
{
	sf::RectangleShape aboveRect(sf::Vector2f(150, 50));
	aboveRect.setPosition(sf::Vector2f(leftTop.x + 50, leftTop.y));
	aboveRect.setFillColor(color);
	window.draw(aboveRect);

	sf::RectangleShape leftRect(sf::Vector2f(50, 300));
	leftRect.setPosition(sf::Vector2f(leftTop.x + 50, leftTop.y));
	leftRect.setFillColor(color);
	window.draw(leftRect);

	sf::RectangleShape lowerRect(sf::Vector2f(100, 50));
	lowerRect.setPosition(sf::Vector2f(leftTop.x, leftTop.y + 250));
	lowerRect.setFillColor(color);
	window.draw(lowerRect);

	sf::RectangleShape rightRect(sf::Vector2f(50, 300));
	rightRect.setPosition(sf::Vector2f(leftTop.x + 150, leftTop.y));
	rightRect.setFillColor(color);
	window.draw(rightRect);
}

void DrawInitials(sf::RenderWindow& window)
{
	sf::Vector2f firstLetterPos{ 50, 100 };
	sf::Vector2f secondLetterPos{ 550, 100 };
	sf::Vector2f thirdLetterPos{ 300, 100 };

	DrawLetterL(window, firstLetterPos, sf::Color::Cyan);
	DrawLetterA(window, secondLetterPos, sf::Color::Magenta);
	DrawLetterC(window, thirdLetterPos, sf::Color::Black);

	window.display();
}

sf::RenderWindow CreateWindow()
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;
	return sf::RenderWindow{
		sf::VideoMode(sf::Vector2u(800, 600)),
		"Initials",
		sf::Style::Default,
		sf::State::Windowed, settings
	};
}