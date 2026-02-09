#pragma once
#include "HangmanViewModel.hpp"
#include "AlphabetView.hpp"
#include <optional>
#include <string>

class GameView
{
public:
	GameView(HangmanViewModel& viewModel, int width, int height);

	void setWindowSize(int width, int height);

	void draw();

	void handleClick(double x, double y);

private:
	HangmanViewModel& m_viewModel;
	AlphabetView m_alphabetView;
	int m_width;
	int m_height;

	void drawGallows() const;
	void drawHangman(int wrongGuesses) const;
	void drawWord(const std::string& word) const;
};
