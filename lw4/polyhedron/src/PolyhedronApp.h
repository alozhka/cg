#pragma once
#include "Cube.hpp"
#include "graphics/light/DirectLight.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class PolyhedronApp final : public GraphicsApplication
{
public:
	explicit PolyhedronApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_cameraController(m_camera)
		, m_light({ 1, 1, 3 })
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");

		m_cube.SetScale(3.2);

		m_light.SetDiffuseIntensity(0.5, 0.5, 0.5);
		m_light.SetAmbientIntensity(0.2, 0.2, 0.2);
		m_light.SetSpecularIntensity(0.3, 0.3, 0.3);
	}

protected:
	void OnDraw(const glm::mat4& projection) override
	{
		glClearColor(0.08, 0.08, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = m_camera.GetViewMatrix();

		m_light.Apply(m_shader, view);

		glm::mat4 mvp = projection * view;
		m_shader.Use();

		m_cube.Draw(m_shader, mvp);
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
	OrbitalCamera m_camera;
	CameraController m_cameraController;
	ShaderProgram m_shader;

	Cube m_cube{};
	DirectLight m_light;
};