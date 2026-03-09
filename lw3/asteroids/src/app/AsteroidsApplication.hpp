#pragma once

#include "../graphics/ShaderProgram.hpp"
#include "../view/GameView.hpp"
#include "../viewmodel/GameViewModel.hpp"
#include "../viewmodel/InputState.hpp"
#include "GuiApplication.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

class AsteroidsApplication : public GuiApplication
{
public:
	AsteroidsApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
		, m_viewModel(static_cast<float>(width), static_cast<float>(height))
	{
		m_shader.LoadFromFile(
			"assets/vertex.glsl",
			"assets/fragment.glsl");
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

protected:
	void OnDraw(const Mat3& projection) override
	{
		glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float dt = GetDeltaTime();
		dt = std::min(dt, 0.05f);

		UpdateWorldSize();

		InputState input = ReadKeyboard();
		m_viewModel.ProcessInput(input);
		m_viewModel.Update(dt);

		glfwSetWindowTitle(GetWindow(), m_viewModel.GetWindowTitle().c_str());

		m_shader.Use();
		m_view.Render(m_shader, projection, m_viewModel);

		if (m_viewModel.IsGameOver() && !m_gameOverShown)
		{
			m_gameOverShown = true;
			ShowGameOverDialog();
		}
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

	InputState ReadKeyboard()
	{
		GLFWwindow* w = GetWindow();
		InputState input;
		input.rotateLeft = glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS;
		input.rotateRight = glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS;
		input.thrust = glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS;
		input.shoot = glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS;
		return input;
	}

	void UpdateWorldSize()
	{
		int w, h;
		glfwGetWindowSize(GetWindow(), &w, &h);
		m_viewModel.SetWorldSize(static_cast<float>(w), static_cast<float>(h));
	}

	void ShowGameOverDialog()
	{
#ifdef _WIN32
		int result = MessageBoxA(
			nullptr,
			("Game Over!\nYour score: " + std::to_string(m_viewModel.GetScore())).c_str(),
			"Asteroids",
			MB_RETRYCANCEL | MB_ICONINFORMATION);

		if (result == IDRETRY)
		{
			m_viewModel.RestartGame();
			m_view.OnRestart();
			m_gameOverShown = false;
		}
		else
		{
			glfwSetWindowShouldClose(GetWindow(), GLFW_TRUE);
		}
#else
		m_viewModel.RestartGame();
		m_view.OnRestart();
		m_gameOverShown = false;
#endif
	}

	ShaderProgram m_shader;
	GameViewModel m_viewModel;
	GameView m_view;
	float m_lastTime = 0;
	bool m_gameOverShown = false;
};
