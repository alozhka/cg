#include "src/view/GameView.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

sf::RenderWindow CreateWindow()
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 16;
	return sf::RenderWindow{
		sf::VideoMode(sf::Vector2u(1024, 700)),
		"The Alchemy",
		sf::Style::Default,
		sf::State::Windowed,
		settings
	};
}

int main()
{
	sf::RenderWindow window = CreateWindow();
	const sf::Font font("/System/Library/Fonts/Geneva.ttf");

	GameView view(window, font);
	view.Run();

	return 0;
}
