#pragma once
#include "../asteroids/view/AsteroidsGameView.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "GuiApplication.h"

class AsteroidsApplication : public GuiApplication
{
public:
	AsteroidsApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
		, m_asteroidsGame(width, height)
		, m_asteroidsGameViewModel(m_asteroidsGame)
		, m_asteroidsGameView(m_asteroidsGameViewModel)
	{
		m_shader.LoadFromFile(
			"assets/vertex.glsl",
			"assets/fragment.glsl");
	}

protected:
	void OnDraw(const Mat3& projection) override
	{
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		float dt = GetDeltaTime();

		m_asteroidsGameView.Draw(m_shader, projection);

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

	AsteroidsGame m_asteroidsGame;
	AsteroidsGameViewModel m_asteroidsGameViewModel;
	AsteroidsGameView m_asteroidsGameView;

	ShaderProgram m_shader;
	float m_lastTime = 0;
};