#pragma once
#include <GLFW/glfw3.h>
#include <vector>

class CurveBezier
{
public:
	explicit CurveBezier(unsigned int segments)
		: m_segments(segments)
	{
		CalculateCurve();
	}

	void Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		DrawStripeLines();
		DrawCurve();
		DrawControlPoints();
	}

private:
	void DrawStripeLines()
	{
		glColor3f(0.5, 0.5, 0.5);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x00FF);
		glBegin(GL_LINE_STRIP);
		for (const Point2D& pt : m_controlPoints)
		{
			glVertex2f(pt.x, pt.y);
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

	void DrawCurve()
	{
		glColor3f(1, 1, 1);
		glLineWidth(2);
		glBegin(GL_LINE_STRIP);
		for (const Point2D& pt : m_curveVertices)
		{
			glVertex2f(pt.x, pt.y);
		}
		glEnd();
	}

	void DrawControlPoints()
	{
		glLineWidth(1.0);
		glColor3f(1, 0, 0);
		glPointSize(12);
		glBegin(GL_POINTS);
		for (const Point2D& pt : m_controlPoints)
		{
			glVertex2f(pt.x, pt.y);
		}
		glEnd();
	}

	void CalculateCurve()
	{
		m_curveVertices.clear();

		for (int i = 0; i <= m_segments; ++i)
		{
			float t = static_cast<float>(i) / static_cast<float>(m_segments);

			float u = 1.0f - t;
			float tt = t * t;
			float uu = u * u;
			float uuu = uu * u;
			float ttt = tt * t;

			Point2D p{};
			p.x = uuu * m_controlPoints[0].x + 3 * uu * t * m_controlPoints[1].x + 3 * u * tt * m_controlPoints[2].x + ttt * m_controlPoints[3].x;
			p.y = uuu * m_controlPoints[0].y + 3 * uu * t * m_controlPoints[1].y + 3 * u * tt * m_controlPoints[2].y + ttt * m_controlPoints[3].y;

			m_curveVertices.push_back(p);
		}
	}

	struct Point2D
	{
		float x, y;
	};

	unsigned int m_segments;
	std::vector<Point2D> m_curveVertices{};
	std::vector<Point2D> m_controlPoints{
		Point2D{ -0.8f, -0.5f },
		Point2D{ -0.4f, 0.8f },
		Point2D{ 0.4f, 0.8f },
		Point2D{ 0.8f, -0.5f }
	};
};