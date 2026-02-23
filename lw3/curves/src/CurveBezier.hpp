#pragma once
#include "shared/Point2D.hpp"
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

	void SetControlPoint(size_t index, double x, double y)
	{
		if (index < m_controlPoints.size())
		{
			m_controlPoints[index].x = x;
			m_controlPoints[index].y = y;
			CalculateCurve();
		}
	}

	std::optional<size_t> GetControlPointIndexAt(Point2D p, double radius) const
	{
		std::optional<size_t> best = std::nullopt;
		double bestDist2 = radius * radius;

		for (size_t i = 0; i < m_controlPoints.size(); ++i)
		{
			double dx = p.x - m_controlPoints[i].x;
			double dy = p.y - m_controlPoints[i].y;
			double d2 = dx * dx + dy * dy;
			if (d2 < bestDist2)
			{
				bestDist2 = d2;
				best = i;
			}
		}

		return best;
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
			glVertex2d(pt.x, pt.y);
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
			glVertex2d(pt.x, pt.y);
		}
		glEnd();
	}

	void DrawControlPoints()
	{
		glLineWidth(1);
		glColor3f(1, 0, 0);
		glPointSize(12);
		glBegin(GL_POINTS);
		for (const Point2D& pt : m_controlPoints)
		{
			glVertex2d(pt.x, pt.y);
		}
		glEnd();
	}

	void CalculateCurve()
	{
		m_curveVertices.clear();

		for (size_t i = 0; i <= m_segments; ++i)
		{
			double t = static_cast<double>(i) / static_cast<double>(m_segments);
			Point2D p = CalculateCoords(t);
			m_curveVertices.push_back(p);
		}
	}

	Point2D CalculateCoords(double t) const
	{
		double u = 1.0 - t;
		double tt = t * t;
		double uu = u * u;
		double uuu = uu * u;
		double ttt = tt * t;

		Point2D p{};

		// B(t) = (1-t)^3*P0 + 3*(1-t)^2*t*P1 + 3*(1-t)^2*t*P2 + t^3*P3
		p.x = uuu * m_controlPoints[0].x
			+ 3 * uu * t * m_controlPoints[1].x
			+ 3 * u * tt * m_controlPoints[2].x
			+ ttt * m_controlPoints[3].x;

		p.y = uuu * m_controlPoints[0].y
			+ 3 * uu * t * m_controlPoints[1].y
			+ 3 * u * tt * m_controlPoints[2].y
			+ ttt * m_controlPoints[3].y;

		return p;
	}

	unsigned int m_segments;
	std::vector<Point2D> m_curveVertices{};
	std::vector<Point2D> m_controlPoints{
		Point2D{ -0.8, -0.5 },
		Point2D{ -0.4, 0.8 },
		Point2D{ 0.4, 0.8 },
		Point2D{ 0.8, -0.5 }
	};
};