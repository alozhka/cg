#pragma once

#include "MobiusToKleinTransform.hpp"

#include <graphics/camera/OrbitalCamera.hpp>
#include <graphics/windows/CameraController.hpp>
#include <graphics/windows/GraphicsApplication.hpp>
#include <graphics/windows/TimeProvider.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

class MorphingApp final : public GraphicsApplication
{
public:
	MorphingApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_controller(m_camera)
	{
		m_camera.SetRadius(4.5);
		m_camera.AddPitch(glm::radians(20.0f));

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.06, 0.07, 0.09, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_elapsed += m_time.GetDeltaTime();
		const glm::mat4 mvp = perspective * m_camera.GetViewMatrix();

		m_transform.Draw(mvp, m_elapsed);
	}

	void OnMouseButton(int button, int action, glm::vec2 p) override
	{
		m_controller.OnMouseButton(button, action, p);
	}

	void OnMouseMove(glm::vec2 p) override
	{
		m_controller.OnMouseMove(p);
	}

	void OnScroll(double, double yOffset) override
	{
		m_controller.OnScroll(yOffset);
	}

private:
	MobiusToKleinTransform m_transform;
	OrbitalCamera m_camera;
	CameraController m_controller;
	TimeProvider m_time;
	float m_elapsed = 0;
};
