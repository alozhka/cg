#pragma once
#include "HangmanViewModel.hpp"
#include "AlphabetView.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class GameView
{
public:
	GameView(HangmanViewModel& viewModel, const sf::Font& font);

	void draw(sf::RenderTarget& target);

	void HandleClick(double x, double y);

private:
	HangmanViewModel& m_viewModel;
	const sf::Font& m_font;
	AlphabetView m_alphabetView;

	static void DrawGallows(sf::RenderTarget& target) ;
	static void DrawHangman(sf::RenderTarget& target, int wrongGuesses) ;
	void DrawWord(sf::RenderTarget& target, const std::string& word) const;
};
