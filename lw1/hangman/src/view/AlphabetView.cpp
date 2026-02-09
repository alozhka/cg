#include "AlphabetView.hpp"
#include "Drawer.hpp"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

AlphabetView::AlphabetView(float startX, float startY, float boxSize, float padding)
	: m_startX(startX)
	, m_startY(startY)
	, m_boxSize(boxSize)
	, m_padding(padding)
{
}

void AlphabetView::draw(const std::vector<int>& alphabetStates) const
{
	for (int i = 0; i < 26; ++i)
	{
		char c = 'A' + i;
		float x = m_startX + (i % m_cols) * (m_boxSize + m_padding);
		float y = m_startY + (i / m_cols) * (m_boxSize + m_padding);

		bool isCorrect = (alphabetStates[i] == 1);
		bool isWrong = (alphabetStates[i] == 2);

		// Draw box background/border
		if (isCorrect)
			glColor3f(0.8f, 1.0f, 0.8f); // Light green
		else if (isWrong)
			glColor3f(1.0f, 0.8f, 0.8f); // Light red
		else
			glColor3f(0.9f, 0.9f, 0.9f); // Gray

		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + m_boxSize, y);
		glVertex2f(x + m_boxSize, y + m_boxSize);
		glVertex2f(x, y + m_boxSize);
		glEnd();

		// Draw border
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + m_boxSize, y);
		glVertex2f(x + m_boxSize, y + m_boxSize);
		glVertex2f(x, y + m_boxSize);
		glEnd();

		// Draw char centered
		Drawer::drawChar(c, x + 5, y + 5, m_boxSize - 10, isCorrect, isWrong);
	}
}

std::optional<char> AlphabetView::getLetterAt(double mouseX, double mouseY) const
{
	for (int i = 0; i < 26; ++i)
	{
		float x = m_startX + (i % m_cols) * (m_boxSize + m_padding);
		float y = m_startY + (i / m_cols) * (m_boxSize + m_padding);

		if (mouseX >= x && mouseX <= x + m_boxSize && mouseY >= y && mouseY <= y + m_boxSize)
		{
			return (char)('A' + i);
		}
	}
	return std::nullopt;
}
