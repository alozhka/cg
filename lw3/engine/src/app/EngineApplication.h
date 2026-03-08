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
		, m_lastTime(GetTime())
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

		m_engine.Update(dt);

		m_shader.Use();
		m_engine.Draw(m_shader, projection);
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

	Engine m_engine{};
	ShaderProgram m_shader;
	float m_lastTime = 0;
};