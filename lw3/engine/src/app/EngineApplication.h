#pragma once
#include "../engine/Engine.hpp"
#include "../render/ShaderProgram.hpp"
#include "../shared/CompositeObject.hpp"
#include "GuiApplication.h"

class EngineApplication : public GuiApplication
{
public:
	EngineApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
	{
		m_shader.LoadFromFile(
			"assets/vertex.glsl",
			"assets/fragment.glsl");
		m_lastTime = glfwGetTime();
	}

protected:
	void OnDraw(const Mat3& projection) override
	{
		glClearColor(0.7, 0.7, 0.7, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		double dt = GetDeltaTime();

		m_engine.Update(dt);

		m_shader.Use();
		m_engine.Draw(m_shader, projection);
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