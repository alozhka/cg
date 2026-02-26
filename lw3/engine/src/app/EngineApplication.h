#pragma once
#include "../engine/EngineAssembly.hpp"
#include "../shared/CompositeObject.hpp"
#include "GuiApplication.h"

class EngineApplication : public GuiApplication
{
public:
	EngineApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
	{
		m_lastTime = glfwGetTime();
	}

protected:
	void OnDraw() override
	{
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
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

	EngineAssembly m_engine{};
	double m_lastTime = 0;
};