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

void DrawLetterC(sf::RenderWindow& window)
{
	sf::RectangleShape leftRect(sf::Vector2f(50, 300));
	leftRect.setPosition(sf::Vector2f(50, 100));
	leftRect.setFillColor(sf::Color::Black);
	window.draw(leftRect);

	sf::RectangleShape aboveRect(sf::Vector2f(200, 50));
	aboveRect.setPosition(sf::Vector2f(50, 100));
	aboveRect.setFillColor(sf::Color::Black);
	window.draw(aboveRect);

	sf::RectangleShape lowerRect(sf::Vector2f(200, 50));
	lowerRect.setPosition(sf::Vector2f(50, 350));
	lowerRect.setFillColor(sf::Color::Black);
	window.draw(lowerRect);
}

void DrawLetterA(sf::RenderWindow& window)
{
	sf::RectangleShape leftRect(sf::Vector2f(50, 300));
	leftRect.setPosition(sf::Vector2f(300, 100));
	leftRect.setFillColor(sf::Color::Black);
	window.draw(leftRect);

	sf::RectangleShape rightRect(sf::Vector2f(50, 300));
	rightRect.setPosition(sf::Vector2f(450, 100));
	rightRect.setFillColor(sf::Color::Black);
	window.draw(rightRect);

	sf::RectangleShape aboveRect(sf::Vector2f(150, 50));
	aboveRect.setPosition(sf::Vector2f(300, 100));
	aboveRect.setFillColor(sf::Color::Black);
	window.draw(aboveRect);

	sf::RectangleShape lowerRect(sf::Vector2f(150, 50));
	lowerRect.setPosition(sf::Vector2f(300, 200));
	lowerRect.setFillColor(sf::Color::Black);
	window.draw(lowerRect);
}

void DrawLetterL(sf::RenderWindow& window)
{
	sf::RectangleShape aboveRect(sf::Vector2f(150, 50));
	aboveRect.setPosition(sf::Vector2f(600, 100));
	aboveRect.setFillColor(sf::Color::Black);
	window.draw(aboveRect);

	sf::RectangleShape leftRect(sf::Vector2f(50, 300));
	leftRect.setPosition(sf::Vector2f(600, 100));
	leftRect.setFillColor(sf::Color::Black);
	window.draw(leftRect);

	sf::RectangleShape lowerRect(sf::Vector2f(100, 50));
	lowerRect.setPosition(sf::Vector2f(550, 350));
	lowerRect.setFillColor(sf::Color::Black);
	window.draw(lowerRect);

	sf::RectangleShape rightRect(sf::Vector2f(50, 300));
	rightRect.setPosition(sf::Vector2f(700, 100));
	rightRect.setFillColor(sf::Color::Black);
	window.draw(rightRect);
}

void DrawInitials(sf::RenderWindow& window)
{
	DrawLetterC(window);
	DrawLetterA(window);
	DrawLetterL(window);
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