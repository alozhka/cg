#pragma once

#include "MazeMesh.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class MazeApp final : public GraphicsApplication
{
public:
	MazeApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_cameraController(m_camera)
		, m_light({ 0, 1, 0 })
	{
		m_zfar = 100.f;

		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		glEnable(GL_DEPTH_TEST);

		float cx = m_mazeMesh.GetWidth() / 2.f;
		float cz = m_mazeMesh.GetDepth() / 2.f;
		m_camera.SetTarget({ cx, 0.f, cz });

		m_light.SetAmbientIntensity(0.3f, 0.3f, 0.3f);
		m_light.SetDiffuseIntensity(0.6f, 0.6f, 0.6f);
		m_light.SetSpecularIntensity(0.2f, 0.2f, 0.2f);
	}

protected:
	void OnDraw(const glm::mat4& projection) override
	{
		glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = m_camera.GetViewMatrix();
		glm::mat4 viewProjection = projection * view;

		m_shader.Use();
		m_light.Apply(m_shader);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());

		m_mazeMesh.DrawFloor(m_shader, viewProjection);
		m_mazeMesh.DrawCeiling(m_shader, viewProjection);
		m_mazeMesh.DrawWalls(m_shader, viewProjection);
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
	DirectLight m_light;
	MazeMesh m_mazeMesh;
};