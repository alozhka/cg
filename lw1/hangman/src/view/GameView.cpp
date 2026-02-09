#include "GameView.hpp"
#include "Drawer.hpp"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

GameView::GameView(HangmanViewModel& viewModel, int width, int height)
	: m_viewModel(viewModel)
	, m_width(width)
	, m_height(height)
	, m_alphabetView(50.0f, 450.0f, 30.0f, 10.0f)
{
}

void GameView::setWindowSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void GameView::draw()
{
	// Setup 2D projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, m_width, m_height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Clear background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw game elements
	drawGallows();
	drawHangman(m_viewModel.getWrongGuesses());
	drawWord(m_viewModel.getDisplayWord());
	
	m_alphabetView.draw(m_viewModel.getAlphabetStates());
}

void GameView::handleClick(double x, double y)
{
	std::optional<char> letter = m_alphabetView.getLetterAt(x, y);
	if (letter)
	{
		m_viewModel.processInput(*letter);
	}
}

void GameView::drawGallows() const
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(3.0f);

	// Base
	Drawer::drawLine(50, 400, 150, 400);
	// Pole
	Drawer::drawLine(100, 400, 100, 100);
	// Top
	Drawer::drawLine(100, 100, 250, 100);
	// Rope
	Drawer::drawLine(250, 100, 250, 150);

	glLineWidth(1.0f);
}

void GameView::drawHangman(int wrongGuesses) const
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);

	if (wrongGuesses >= 1)
	{ // Head
		Drawer::drawCircle(250, 170, 20, 20);
	}
	if (wrongGuesses >= 2)
	{ // Body
		Drawer::drawLine(250, 190, 250, 270);
	}
	if (wrongGuesses >= 3)
	{ // Left Arm
		Drawer::drawLine(250, 210, 220, 240);
	}
	if (wrongGuesses >= 4)
	{ // Right Arm
		Drawer::drawLine(250, 210, 280, 240);
	}
	if (wrongGuesses >= 5)
	{ // Left Leg
		Drawer::drawLine(250, 270, 230, 310);
	}
	if (wrongGuesses >= 6)
	{ // Right Leg
		Drawer::drawLine(250, 270, 270, 310);
	}
}

void GameView::drawWord(const std::string& word) const
{
	float startX = 400;
	float startY = 300;
	float charSize = 20;
	float spacing = 30;

	for (size_t i = 0; i < word.length(); ++i)
	{
		Drawer::drawChar(word[i], startX + i * spacing, startY, charSize, false, false);
	}
}
