#pragma once
#include "../camera/OrbitalCamera.hpp"
#include <GLFW/glfw3.h>

class CameraController
{
public:
	explicit CameraController(OrbitalCamera& camera)
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
		m_camera.AddPitch(delta.y * m_sensitivity);
	}

	void OnScroll(double yOffset)
	{
		m_camera.AddRadius(-static_cast<float>(yOffset) * m_zoomSensitivity);
	}

private:
	OrbitalCamera& m_camera;
	bool m_isDragging = false;
	glm::vec2 m_lastMousePos = { 0, 0 };
	float m_sensitivity = 1.5;
	float m_zoomSensitivity = 0.3;
};