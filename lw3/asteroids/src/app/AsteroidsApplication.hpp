#pragma once
#include "../asteroids/view/AsteroidsGameView.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "GuiApplication.h"
#include "KeyboardReader.hpp"

#include <string>

class AsteroidsApplication final : public GuiApplication
{
public:
	AsteroidsApplication(int width, int height, const std::string& title)
		: GuiApplication(width, height, title)
		, m_asteroidsGame(std::make_shared<AsteroidsGame>(width, height))
		, m_asteroidsGameViewModel(m_asteroidsGame)
		, m_asteroidsGameView(m_asteroidsGameViewModel)
		, m_keyboard(CreateKeyboardReader())
	{
		m_shader.LoadFromFile(
			"assets/vertex.glsl",
			"assets/fragment.glsl");
		m_asteroidsGame->StartGame();
	}

protected:
	void OnDraw(const Mat3& projection) override
	{
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		Update();
		UpdateWindowTitle();
		m_asteroidsGameView.Draw(m_shader, projection);

		m_shader.Use();
	}

private:
	void Update()
	{
		float dt = GetDeltaTime();
		HandleKeyboardClicks();
		m_asteroidsGame->Update(dt);
	}

	void UpdateWindowTitle()
	{
		auto state = m_asteroidsGame->GetState();
		int lives = m_asteroidsGame->GetLives();
		int score = m_asteroidsGame->GetScore();

		std::string title = "Asteroids";
		if (state == AsteroidsGame::GameState::Playing)
		{
			title += " | Lives: " + std::to_string(lives) + " | Score: " + std::to_string(score);
		}
		else if (state == AsteroidsGame::GameState::GameOver)
		{
			title += " | Game Over | Final Score: " + std::to_string(score) + " | Press R to restart";
		}
		else if (state == AsteroidsGame::GameState::Idle)
		{
			title += " | Ready";
		}
		SetWindowTitle(title);
	}

	void HandleKeyboardClicks()
	{
		HandleGameActions();
		HandleSpaceshipActions();
	}

	void HandleGameActions()
	{
		if (m_asteroidsGame->GetState() == AsteroidsGame::GameState::GameOver)
		{
			if (m_keyboard.IsButtonPressed(GLFW_KEY_R))
			{
				m_asteroidsGame->StartGame();
			}
		}
	}

	void HandleSpaceshipActions()
	{
		bool shouldThrust = m_keyboard.IsButtonPressed(GLFW_KEY_UP);
		if (shouldThrust)
		{
			m_asteroidsGame->GetSpaceship()->BeginThrust();
		}
		else
		{
			m_asteroidsGame->GetSpaceship()->FinishThrust();
		}

		bool shouldRotateLeft = m_keyboard.IsButtonPressed(GLFW_KEY_LEFT);
		if (shouldRotateLeft)
		{
			m_asteroidsGame->GetSpaceship()->ApplyLeftRotation();
		}

		bool shouldRotateRight = m_keyboard.IsButtonPressed(GLFW_KEY_RIGHT);
		if (shouldRotateRight)
		{
			m_asteroidsGame->GetSpaceship()->ApplyRightRotation();
		}

		bool shouldShoot = m_keyboard.IsButtonPressed(GLFW_KEY_SPACE);
		if (shouldShoot)
		{
			m_asteroidsGame->FireBullet();
		}
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

	AsteroidsGamePtr m_asteroidsGame;
	AsteroidsGameViewModel m_asteroidsGameViewModel;
	AsteroidsGameView m_asteroidsGameView;

	KeyboardReader m_keyboard;
	ShaderProgram m_shader;
	float m_lastTime = 0;
};