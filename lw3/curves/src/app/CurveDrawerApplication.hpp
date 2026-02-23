#pragma once

#include "../CurveBezier.hpp"
#include "../shared/GuiApplication.h"

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

private:
	CurveBezier m_bezierCurve;
};