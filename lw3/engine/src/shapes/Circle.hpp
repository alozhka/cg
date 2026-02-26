#pragma once
#include "../shared/Color.hpp"
#include "../shared/SceneObject.hpp"
#include <numbers>

class Circle : public SceneObject
{
public:
	Circle(Point center, Color color, float radius, int segments = 30)
		: SceneObject(center, 0, 1)
		, m_radius(radius)
		, m_segments(segments)
		, m_color(color)
	{
	}

	void Draw() override
	{
		glPushMatrix();
		ApplyTransform();

		m_color.ApplyColor();
		glBegin(GL_TRIANGLE_FAN);
		glVertex2d(0, 0);
		for (int i = 0; i <= m_segments; ++i)
		{
			double theta = 2 * std::numbers::pi * static_cast<double>(i) / static_cast<double>(m_segments);
			double dx = m_radius * std::cos(theta);
			double dy = m_radius * std::sin(theta);
			glVertex2d(dx, dy);
		}
		glEnd();

		glPopMatrix();
	}

private:
	float m_radius;
	int m_segments;
	Color m_color;
};