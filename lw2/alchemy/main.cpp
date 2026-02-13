#include "src/view/GameView.h"

#include <SFML/Graphics/RenderWindow.hpp>

sf::RenderWindow CreateWindow()
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 16;
	return sf::RenderWindow{
		sf::VideoMode(sf::Vector2u(800, 600)),
		"The Alchemy",
		sf::Style::Default,
		sf::State::Windowed,
		settings
	};
}

int main()
{
	sf::RenderWindow window = CreateWindow();
	GameView view(window);

	view.Run();

	return 0;
}
