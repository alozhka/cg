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
		auto engine = std::make_shared<EngineAssembly>();
		engine->SetScale(0.002);

		m_root.AddChild(engine);
		m_lastTime = glfwGetTime();
	}

protected:
	void OnDraw() override
	{
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		double t = glfwGetTime();
		double dt = t - m_lastTime;
		m_lastTime = t;

		m_root.Update(dt);
		m_root.Draw();
	}

private:
	CompositeObject m_root{};
	double m_lastTime = 0;
};