#pragma once
#include "../graphics/ShaderProgram.hpp"
#include "GuiApplication.h"

class AsteroidsApplication : public GuiApplication
{
public:
	AsteroidsApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
	{
		m_shader.LoadFromFile(
			"assets/vertex.glsl",
			"assets/fragment.glsl");
	}

protected:
	void OnDraw(const Mat3& projection) override
	{
		glClearColor(0.7, 0.7, 0.7, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		float dt = GetDeltaTime();

		m_shader.Use();
	}

private:
	float GetDeltaTime()
	{
		auto t = GetTime();
		float dt = t - m_lastTime;
		m_lastTime = t;

		return dt;
	}

	static float GetTime()
	{
		return static_cast<float>(glfwGetTime());
	}

	ShaderProgram m_shader;
	float m_lastTime = 0;
};