#pragma once
#include "../render/ShaderProgram.hpp"
#include "GuiApplication.h"
#include "../engine/Engine.hpp"
#include "../shared/CompositeObject.hpp"

class EngineApplication : public GuiApplication
{
public:
	EngineApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
	{
		m_shader.LoadFromFile(
			"assets/vertex.glsl",
			"assets/fragment.glsl");
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
	ShaderProgram m_shader;
	double m_lastTime = 0;
};