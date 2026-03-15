#pragma once
#include "../asteroids/view/AsteroidsGameView.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "GuiApplication.h"
#include "KeyboardReader.hpp"

class AsteroidsApplication final : public GuiApplication
{
public:
	AsteroidsApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
		, m_asteroidsGame(width, height)
		, m_asteroidsGameViewModel(m_asteroidsGame)
		, m_asteroidsGameView(m_asteroidsGameViewModel)
		, m_keyboard(CreateKeyboardReader())
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

		Update();
		m_asteroidsGameView.Draw(m_shader, projection);

		m_shader.Use();
	}

private:
	void Update()
	{
		float dt = GetDeltaTime();
		UpdateSpaceship(dt);
	}

	void UpdateSpaceship(float dt)
	{
		bool shouldThrust = m_keyboard.IsButtonPressed(GLFW_KEY_UP);
		if (shouldThrust)
		{
			m_asteroidsGame.GetSpaceship()->ApplyThrust(dt);
		}

		bool shouldRotateLeft = m_keyboard.IsButtonPressed(GLFW_KEY_LEFT);
		if (shouldRotateLeft)
		{
			m_asteroidsGame.GetSpaceship()->ApplyLeftRotation();
		}

		bool shouldRotateRight = m_keyboard.IsButtonPressed(GLFW_KEY_RIGHT);
		if (shouldRotateRight)
		{
			m_asteroidsGame.GetSpaceship()->ApplyRightRotation();
		}

		m_asteroidsGame.GetSpaceship()->Update(dt);
	}

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

	KeyboardReader m_keyboard;
	ShaderProgram m_shader;
	float m_lastTime = 0;
};