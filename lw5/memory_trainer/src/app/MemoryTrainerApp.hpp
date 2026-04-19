#pragma once
#include "../memory_trainer/model/MemoryTrainer.hpp"
#include "../memory_trainer/view/MemoryTrainerView.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class MemoryTrainerApp final : public GraphicsApplication
{
public:
	MemoryTrainerApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_viewModel(m_game)
		, m_view(m_viewModel)
	{
		stbi_set_flip_vertically_on_load(true);
		m_shader.LoadFromFile("assets/textured.vert", "assets/textured.frag");
		glEnable(GL_DEPTH_TEST);

		m_light.SetDiffuseIntensity(0.7, 0.7, 0.7);
		m_light.SetAmbientIntensity(0.55, 0.55, 0.55);
		m_light.SetSpecularIntensity(0.2, 0.2, 0.2);
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.05, 0.05, 0.08, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader.Use();
		m_light.Apply(m_shader);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());

		glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 viewProjection = perspective * view;

		m_view.Draw(m_shader, viewProjection);
	}

	void OnMouseButton(int button, int action, glm::vec2 p) override
	{
		m_cameraController.OnMouseButton(button, action, p);
	}

	void OnMouseMove(glm::vec2 p) override
	{
		m_cameraController.OnMouseMove(p);
	}

private:
	ShaderProgram m_shader;

	OrbitalCamera m_camera;
	CameraController m_cameraController{ m_camera };
	DirectLight m_light{ { 1, 0, 0 } };

	MemoryTrainerPtr m_game = std::make_shared<MemoryTrainer>();
	MemoryTrainerViewModel m_viewModel;
	MemoryTrainerView m_view;
};
