#pragma once

#include "../../../vendors/graphics/camera/FirstPersonCamera.hpp"
#include <GLFW/glfw3.h>

class FirstPersonCameraController
{
public:
	explicit FirstPersonCameraController(FirstPersonCamera& camera)
		: m_camera(camera)
	{
	}

	void OnMouseButton(int button, int action, glm::vec2 p)
	{
		if (button != GLFW_MOUSE_BUTTON_LEFT)
		{
			return;
		}

		if (action == GLFW_PRESS)
		{
			m_isDragging = true;
			m_lastMousePos = p;
		}

		if (action == GLFW_RELEASE)
		{
			m_isDragging = false;
		}
	}

	void OnMouseMove(glm::vec2 p)
	{
		if (!m_isDragging)
		{
			return;
		}

		glm::vec2 delta = p - m_lastMousePos;
		m_lastMousePos = p;

		m_camera.AddYaw(-delta.x * m_sensitivity);
		m_camera.AddPitch(-delta.y * m_sensitivity);
	}

private:
	FirstPersonCamera& m_camera;
	bool m_isDragging = false;
	glm::vec2 m_lastMousePos{ 0.f, 0.f };
	float m_sensitivity = 1.5f;
};