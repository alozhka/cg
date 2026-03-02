#pragma once
#include "../engine/Engine.hpp"
#include "../shared/CompositeObject.hpp"
#include "GuiApplication.h"

class EngineApplication : public GuiApplication
{
public:
	EngineApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
	{
		m_engine.SetScale(1.2);
		m_lastTime = glfwGetTime();
	}

protected:
	void OnDraw() override
	{
		glClearColor(0.7, 0.7, 0.7, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		double dt = GetDeltaTime();

		m_engine.Update(dt);
		m_engine.Draw();
	}

private:
	double GetDeltaTime()
	{
		double t = glfwGetTime();
		double dt = t - m_lastTime;
		m_lastTime = t;

		return dt;
	}

	Engine m_engine{};
	double m_lastTime = 0;
};