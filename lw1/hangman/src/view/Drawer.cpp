#include "Drawer.hpp"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <cmath>

void Drawer::drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void Drawer::drawCircle(float cx, float cy, float r, int segments)
{
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(segments);
		float x = r * cosf(theta);
		float y = r * sinf(theta);
		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}

void Drawer::drawChar(char c, float x, float y, float size, bool isCorrect, bool isWrong)
{
	if (isCorrect)
		glColor3f(0.0f, 0.5f, 0.0f);
	else if (isWrong)
		glColor3f(0.8f, 0.0f, 0.0f);
	else
		glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size / 10.0f, size / 10.0f, 1.0f);

	glBegin(GL_LINES);
	switch (c)
	{
	case 'A':
		glVertex2f(0, 10); glVertex2f(5, 0);
		glVertex2f(5, 0); glVertex2f(10, 10);
		glVertex2f(2, 6); glVertex2f(8, 6);
		break;
	case 'B':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 0); glVertex2f(8, 0);
		glVertex2f(8, 0); glVertex2f(8, 5);
		glVertex2f(8, 5); glVertex2f(0, 5);
		glVertex2f(0, 5); glVertex2f(8, 5);
		glVertex2f(8, 5); glVertex2f(8, 10);
		glVertex2f(8, 10); glVertex2f(0, 10);
		break;
	case 'C':
		glVertex2f(10, 0); glVertex2f(0, 0);
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(10, 10);
		break;
	case 'D':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 0); glVertex2f(8, 2);
		glVertex2f(8, 2); glVertex2f(8, 8);
		glVertex2f(8, 8); glVertex2f(0, 10);
		break;
	case 'E':
		glVertex2f(10, 0); glVertex2f(0, 0);
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(10, 10);
		glVertex2f(0, 5); glVertex2f(8, 5);
		break;
	case 'F':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(0, 5); glVertex2f(8, 5);
		break;
	case 'G':
		glVertex2f(10, 0); glVertex2f(0, 0);
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(10, 10);
		glVertex2f(10, 10); glVertex2f(10, 5);
		glVertex2f(10, 5); glVertex2f(5, 5);
		break;
	case 'H':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(10, 0); glVertex2f(10, 10);
		glVertex2f(0, 5); glVertex2f(10, 5);
		break;
	case 'I':
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(5, 0); glVertex2f(5, 10);
		glVertex2f(0, 10); glVertex2f(10, 10);
		break;
	case 'J':
		glVertex2f(10, 0); glVertex2f(10, 10);
		glVertex2f(10, 10); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(0, 5);
		break;
	case 'K':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 5); glVertex2f(10, 0);
		glVertex2f(0, 5); glVertex2f(10, 10);
		break;
	case 'L':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(10, 10);
		break;
	case 'M':
		glVertex2f(0, 10); glVertex2f(0, 0);
		glVertex2f(0, 0); glVertex2f(5, 5);
		glVertex2f(5, 5); glVertex2f(10, 0);
		glVertex2f(10, 0); glVertex2f(10, 10);
		break;
	case 'N':
		glVertex2f(0, 10); glVertex2f(0, 0);
		glVertex2f(0, 0); glVertex2f(10, 10);
		glVertex2f(10, 10); glVertex2f(10, 0);
		break;
	case 'O':
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(10, 0); glVertex2f(10, 10);
		glVertex2f(10, 10); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(0, 0);
		break;
	case 'P':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(10, 0); glVertex2f(10, 5);
		glVertex2f(10, 5); glVertex2f(0, 5);
		break;
	case 'Q':
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(10, 0); glVertex2f(10, 10);
		glVertex2f(10, 10); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(0, 0);
		glVertex2f(5, 5); glVertex2f(10, 10);
		break;
	case 'R':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(10, 0); glVertex2f(10, 5);
		glVertex2f(10, 5); glVertex2f(0, 5);
		glVertex2f(0, 5); glVertex2f(10, 10);
		break;
	case 'S':
		glVertex2f(10, 0); glVertex2f(0, 0);
		glVertex2f(0, 0); glVertex2f(0, 5);
		glVertex2f(0, 5); glVertex2f(10, 5);
		glVertex2f(10, 5); glVertex2f(10, 10);
		glVertex2f(10, 10); glVertex2f(0, 10);
		break;
	case 'T':
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(5, 0); glVertex2f(5, 10);
		break;
	case 'U':
		glVertex2f(0, 0); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(10, 10);
		glVertex2f(10, 10); glVertex2f(10, 0);
		break;
	case 'V':
		glVertex2f(0, 0); glVertex2f(5, 10);
		glVertex2f(5, 10); glVertex2f(10, 0);
		break;
	case 'W':
		glVertex2f(0, 0); glVertex2f(2, 10);
		glVertex2f(2, 10); glVertex2f(5, 5);
		glVertex2f(5, 5); glVertex2f(8, 10);
		glVertex2f(8, 10); glVertex2f(10, 0);
		break;
	case 'X':
		glVertex2f(0, 0); glVertex2f(10, 10);
		glVertex2f(10, 0); glVertex2f(0, 10);
		break;
	case 'Y':
		glVertex2f(0, 0); glVertex2f(5, 5);
		glVertex2f(10, 0); glVertex2f(5, 5);
		glVertex2f(5, 5); glVertex2f(5, 10);
		break;
	case 'Z':
		glVertex2f(0, 0); glVertex2f(10, 0);
		glVertex2f(10, 0); glVertex2f(0, 10);
		glVertex2f(0, 10); glVertex2f(10, 10);
		break;
	case '_':
		glVertex2f(0, 10); glVertex2f(10, 10);
		break;
	default:
		break;
	}
	glEnd();

	glPopMatrix();
}
