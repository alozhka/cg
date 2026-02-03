#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <numbers>

namespace
{
constexpr float BASE_Y = 100, AMPLITUDE = 50, PERIOD = 1.2;
constexpr float ANGULAR_SPEED = 2 * std::numbers::pi / PERIOD;
} // namespace

int MainLoop(sf::RenderWindow& window, const std::function<void()>& onDraw);
sf::RenderWindow CreateWindow();
void DrawBouncingInitials(sf::RenderWindow& window, float timeInSeconds);

int main()
{
	sf::RenderWindow window = CreateWindow();
	sf::Clock clock;

	std::function onDraw = [&] {
		float time = clock.getElapsedTime().asSeconds();
		window.clear(sf::Color::White);
		DrawBouncingInitials(window, time);
	};

	return MainLoop(window, onDraw);
}

int MainLoop(sf::RenderWindow& window, const std::function<void()>& onDraw)
{
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

		onDraw();
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

void DrawBouncingInitials(sf::RenderWindow& window, float timeInSeconds)
{
	constexpr float phase1 = 0;
	constexpr float phase2 = 2 * std::numbers::pi / 3;
	constexpr float phase3 = 4 * std::numbers::pi / 3;

	// y = y0 + A * sin(ωt + φ)
	float y1 = BASE_Y + AMPLITUDE * std::sin(ANGULAR_SPEED * timeInSeconds + phase1);
	float y2 = BASE_Y + AMPLITUDE * std::sin((ANGULAR_SPEED + 0.2) * timeInSeconds + phase2);
	float y3 = BASE_Y + AMPLITUDE * std::sin((ANGULAR_SPEED + 0.3) * timeInSeconds + phase3);

	sf::Vector2f firstLetterPos{ 50, y1 };
	sf::Vector2f secondLetterPos{ 300, y3 };
	sf::Vector2f thirdLetterPos{ 550, y2 };

	DrawLetterL(window, firstLetterPos, sf::Color::Cyan);
	DrawLetterC(window, secondLetterPos, sf::Color::Black);
	DrawLetterA(window, thirdLetterPos, sf::Color::Magenta);

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
		sf::State::Windowed,
		settings
	};
}