#pragma once
#include "MobiusStrip.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class MobiusStripApp final : public GraphicsApplication
{
public:
	MobiusStripApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_cameraController(m_camera)
		, m_light({ 1, 2, 3 })
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		glEnable(GL_DEPTH_TEST);

		m_light.SetDiffuseIntensity(0.7, 0.7, 0.7);
		m_light.SetAmbientIntensity(0.25, 0.25, 0.25);
		m_light.SetSpecularIntensity(0.5, 0.5, 0.5);
	}

protected:
	void OnDraw(const glm::mat4& projection) override
	{
		glClearColor(0.05, 0.05, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);

		m_shader.Use();
		m_light.Apply(m_shader);
		m_shader.SetUniformVec3("uCameraPos", m_camera.GetPosition());

		glm::mat4 vp = projection * m_camera.GetViewMatrix();
		m_mobiusStrip.Draw(m_shader, vp);
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
	MobiusStrip m_mobiusStrip{ 2, 2 };
	DirectLight m_light;
};