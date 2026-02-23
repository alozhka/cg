#pragma once

#include "../CurveBezier.hpp"
#include "../shared/GuiApplication.h"
#include <GLFW/glfw3.h>

class CurveDrawerApplication : public GuiApplication
{
public:
	CurveDrawerApplication()
		: GuiApplication(800, 600, "Curves drawer")
		, m_bezierCurve(100)
	{
	}

	void OnDraw() override
	{
		m_bezierCurve.Draw();
	}

	void OnMouseButton(int button, int action, Point2D p) override
	{
		if (button != GLFW_MOUSE_BUTTON_LEFT)
		{
			return;
		}
		if (action == GLFW_PRESS)
		{
			m_draggingPoint = m_bezierCurve.GetControlPointIndexAt(p, 0.08);
		}
		else if (action == GLFW_RELEASE)
		{
			m_draggingPoint = std::nullopt;
		}
	}

	void OnMouseMove(Point2D p) override
	{
		if (!m_draggingPoint.has_value())
		{
			return;
		}

		m_bezierCurve.SetControlPoint(m_draggingPoint.value(), p.x, p.y);
	}

private:
	CurveBezier m_bezierCurve;
	std::optional<size_t> m_draggingPoint = std::nullopt;
};