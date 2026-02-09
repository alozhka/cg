#include "src/view/GameView.h"
#include <iostream>

sf::RenderWindow CreateWindow()
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 16;
	return sf::RenderWindow{
		sf::VideoMode(sf::Vector2u(800, 600)),
		"The Hangman Game",
		sf::Style::Default,
		sf::State::Windowed,
		settings
	};
}

int main()
{
	sf::RenderWindow window = CreateWindow();
	sf::Font font;

	if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf"))
	{
		std::cerr << "Failed to load font. Make sure a font is available at standard paths.\n";
		return 1;
	}

	HangmanModel hangmanModel("Thatcher");
	HangmanViewModel hangmanViewModel(hangmanModel);
	GameView view(window, font, hangmanViewModel);
	view.Run();
	return 0;
}
