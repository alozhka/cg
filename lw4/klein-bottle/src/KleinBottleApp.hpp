#pragma once

#include "KleinBottle.hpp"
#include "MobiusStrip.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/light/DirectLight.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

class KleinBottleApp final : public GraphicsApplication
{
public:
	KleinBottleApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_cameraController(m_camera)
		, m_light({ 1.f, 2.f, 3.f })
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		glEnable(GL_DEPTH_TEST);

		m_light.SetDiffuseIntensity(0.7f, 0.7f, 0.7f);
		m_light.SetAmbientIntensity(0.25f, 0.25f, 0.25f);
		m_light.SetSpecularIntensity(0.5f, 0.5f, 0.5f);
	}

protected:
	void OnDraw(const glm::mat4& projection) override
	{
		glClearColor(0.05f, 0.05f, 0.1f, 1.f);
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
	MobiusStrip m_mobiusStrip{2, 2};
	DirectLight m_light;
};
