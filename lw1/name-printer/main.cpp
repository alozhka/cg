#include "BouncingObject.h"
#include "Letters.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <optional>

constexpr float GROUND_Y = 450.0f;

sf::RenderWindow CreateWindow()
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;
	return sf::RenderWindow{
		sf::VideoMode(sf::Vector2u(800, 600)),
		"Initials",
		sf::Style::Default,
		sf::State::Windowed,
		settings
	};
}

int MainLoop(const std::function<void(sf::RenderWindow& window)>& onDraw)
{
	sf::RenderWindow window = CreateWindow();

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

		window.clear(sf::Color::White);
		onDraw(window);
		window.display();
	}

	return 0;
}

void DrawLetters(sf::RenderWindow& window, const std::array<BouncingObject, 3>& letters)
{
	sf::Vector2f firstPos{ 50, letters[0].GetY() - Letters::LETTER_HEIGHT };
	sf::Vector2f secondPos{ 300, letters[1].GetY() - Letters::LETTER_HEIGHT };
	sf::Vector2f thirdPos{ 550, letters[2].GetY() - Letters::LETTER_HEIGHT };

	Letters::DrawLetterL(window, firstPos, sf::Color::Cyan);
	Letters::DrawLetterC(window, secondPos, sf::Color::Black);
	Letters::DrawLetterA(window, thirdPos, sf::Color::Magenta);
}

int main()
{
	sf::Clock clock;
	std::array<BouncingObject, 3> letters = {
		BouncingObject({ 50, GROUND_Y }, -320.0f),
		BouncingObject({ 300, GROUND_Y }, -280.0f),
		BouncingObject({ 550, GROUND_Y }, -240.0f)
	};

	auto onDraw = [&clock, &letters](sf::RenderWindow& window) {
		float elapsedTime = clock.getElapsedTime().asSeconds();

		for (auto& letter : letters)
		{
			letter.update(elapsedTime);
		}

		DrawLetters(window, letters);
	};

	return MainLoop(onDraw);
}
