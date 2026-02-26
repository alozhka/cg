#pragma once
#include "../shared/Color.hpp"
#include "../shared/SceneObject.hpp"

class Rectangle : public SceneObject
{
public:
	Rectangle(Point p, double width, double height, const Color& color, double rotation = 0)
		: SceneObject(p, rotation, 1)
		, m_width(width)
		, m_height(height)
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