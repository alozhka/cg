#pragma once
#include "../shared/Color.hpp"
#include "../shared/Rect.h"
#include "../shared/SceneObject.hpp"

class Rectangle : public SceneObject
{
public:
	Rectangle(const Rect& rect, const Color& color, double rotation = 0)
		: SceneObject({ rect.x, rect.y }, rotation, 1)
		, m_width(rect.width)
		, m_height(rect.height)
		, m_color(color)
	{
	}

	void Draw() override
	{
		glPushMatrix();
		ApplyTransform();

		m_color.ApplyColor();
		glBegin(GL_QUADS);

		double centerX = m_width / 2;
		double centerY = m_height / 2;

		glVertex2d(-centerX, -centerY);
		glVertex2d(centerX, -centerY);
		glVertex2d(centerX, centerY);
		glVertex2d(-centerX, centerY);
		glEnd();

		glPopMatrix();
	}

private:
	double m_width;
	double m_height;
	Color m_color;
};