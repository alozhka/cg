#pragma once
#include "../memory_trainer/model/MemoryTrainer.hpp"
#include "../memory_trainer/view/MemoryTrainerView.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/TimeProvider.hpp>

class MemoryTrainerApp final : public GraphicsApplication
{
public:
	MemoryTrainerApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_baseTitle(title)
		, m_keyboard(CreateKeyboardReader())
		, m_viewModel(m_game)
		, m_view(m_viewModel)
	{
		stbi_set_flip_vertically_on_load(true);
		m_shader.LoadFromFile("assets/textured.vert", "assets/textured.frag");
		glEnable(GL_DEPTH_TEST);

		m_light.SetDiffuseIntensity(0.7, 0.7, 0.7);
		m_light.SetAmbientIntensity(0.3, 0.3, 0.3);
		m_light.SetSpecularIntensity(0.2, 0.2, 0.2);
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.05, 0.05, 0.08, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Update();

		m_shader.Use();
		m_light.Apply(m_shader);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());

		glm::mat4 view = m_camera.GetViewMatrix();
		m_viewProjection = perspective * view;

		m_view.Draw(m_shader, m_viewProjection);
	}

	void Update()
	{
		float dt = m_time.GetDeltaTime();
		HandleKeyboardClicks();
		m_game->Update(dt);
		UpdateWindowTitle();
	}

	void HandleKeyboardClicks()
	{
		if (m_keyboard.IsButtonPressed(GLFW_KEY_R))
		{
			m_game->Reset();
			m_camera.SetTarget({0, 0, 0});
		}
	}

	void UpdateWindowTitle()
	{
		std::string title = m_baseTitle;
		if (m_game->IsWon())
		{
			title += " | You won! Press R to restart";
		}

		SetWindowTitle(title);
	}

	void OnMouseButton(int button, int action, glm::vec2 p) override
	{
		m_cameraController.OnMouseButton(button, action, p);

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			auto [origin, dir] = BuildRay(p, m_viewProjection);
			m_viewModel.TryPick(origin, dir);
		}
	}

	void OnMouseMove(glm::vec2 p) override
	{
		m_cameraController.OnMouseMove(p);
	}

private:
	std::string m_baseTitle;
	KeyboardReader m_keyboard;

	glm::mat4 m_viewProjection{};
	ShaderProgram m_shader;
	OrbitalCamera m_camera;
	CameraController m_cameraController{ m_camera };
	DirectLight m_light{ { 1, 0, 0 } };

	TimeProvider m_time;
	MemoryTrainerPtr m_game = std::make_shared<MemoryTrainer>();
	MemoryTrainerViewModel m_viewModel;
	MemoryTrainerView m_view;
};
