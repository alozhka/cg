#include "GameView.hpp"
#include "Drawer.hpp"

GameView::GameView(HangmanViewModel& viewModel, const sf::Font& font)
	: m_viewModel(viewModel)
	, m_font(font)
	, m_alphabetView(font, 50.0f, 450.0f, 30.0f, 10.0f)
{
}

void GameView::draw(sf::RenderTarget& target)
{
	DrawGallows(target);
	DrawHangman(target, m_viewModel.getWrongGuesses());
	DrawWord(target, m_viewModel.getDisplayWord());

	m_alphabetView.draw(target, m_viewModel.getAlphabetStates());
}

void GameView::HandleClick(double x, double y)
{
	std::optional<char> letter = m_alphabetView.getLetterAt(x, y);
	if (letter)
	{
		m_viewModel.processInput(*letter);
	}
}

void GameView::DrawGallows(sf::RenderTarget& target)
{
	Drawer::drawLine(target, 50, 400, 150, 400, 3.0f);
	Drawer::drawLine(target, 100, 400, 100, 100, 3.0f);
	Drawer::drawLine(target, 100, 100, 250, 100, 3.0f);
	Drawer::drawLine(target, 250, 100, 250, 150, 3.0f);
}

void GameView::DrawHangman(sf::RenderTarget& target, int wrongGuesses)
{
	float thickness = 2.0f;
	if (wrongGuesses >= 1)
	{ // Head
		Drawer::drawCircle(target, 250, 170, 20, 20, thickness);
	}
	if (wrongGuesses >= 2)
	{ // Body
		Drawer::drawLine(target, 250, 190, 250, 270, thickness);
	}
	if (wrongGuesses >= 3)
	{ // Left Arm
		Drawer::drawLine(target, 250, 210, 220, 240, thickness);
	}
	if (wrongGuesses >= 4)
	{ // Right Arm
		Drawer::drawLine(target, 250, 210, 280, 240, thickness);
	}
	if (wrongGuesses >= 5)
	{ // Left Leg
		Drawer::drawLine(target, 250, 270, 230, 310, thickness);
	}
	if (wrongGuesses >= 6)
	{ // Right Leg
		Drawer::drawLine(target, 250, 270, 270, 310, thickness);
	}
}

void GameView::DrawWord(sf::RenderTarget& target, const std::string& word) const
{
	float startX = 400;
	float startY = 300;
	float charSize = 20;
	float spacing = 30;

	for (size_t i = 0; i < word.length(); ++i)
	{
		sf::Text text(m_font);
		text.setString(std::string(1, word[i]));
		text.setCharacterSize(static_cast<unsigned int>(charSize));
		text.setFillColor(sf::Color::Black);

		sf::FloatRect bounds = text.getLocalBounds();
		float textCenterX = bounds.position.x + bounds.size.x / 2.0f;
		float textCenterY = bounds.position.y + bounds.size.y / 2.0f;

		text.setPosition(sf::Vector2f(startX + i * spacing - textCenterX + charSize / 2.0f, startY - textCenterY + charSize / 2.0f));

		target.draw(text);
	}
}
