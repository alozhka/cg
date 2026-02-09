#pragma once
#include "HangmanViewModel.hpp"
#include <optional>
#include <string>
#include <vector>

class GameView
{
public:
	GameView(HangmanViewModel& viewModel, int width, int height);

	void setWindowSize(int width, int height);

	void draw();

	void handleClick(double x, double y);

	std::optional<char> getLetterAt(double x, double y) const;

private:
	HangmanViewModel& m_viewModel;
	int m_width;
	int m_height;

	void drawGallows() const;
	void drawHangman(int wrongGuesses) const;
	void drawWord(const std::string& word) const;
	void drawAlphabet(const std::vector<int>& alphabetStates) const;

	// Helper to draw a single character using lines
	void drawChar(char c, float x, float y, float size, bool isCorrect, bool isWrong) const;

	// Helper to draw a line
	void drawLine(float x1, float y1, float x2, float y2) const;
	void drawCircle(float cx, float cy, float r, int segments) const;
};
