#pragma once

#include "../camera/FirstPersonCamera.hpp"

class FirstPersonCameraController
{
public:
	explicit FirstPersonCameraController(FirstPersonCamera& camera)
		: m_camera(camera)
	{
	}

	void SetCursorPosition(glm::vec2 pos)
	{
		m_lastX = pos.x;
		m_lastY = pos.y;
	}

	void OnRawMouseMove(double x, double y)
	{
		double dx = x - m_lastX;
		double dy = y - m_lastY;
		m_lastX = x;
		m_lastY = y;

		m_camera.AddYaw(static_cast<float>(dx) * m_sensitivity);
		m_camera.AddPitch(-static_cast<float>(dy) * m_sensitivity);
	}

private:
	FirstPersonCamera& m_camera;
	double m_lastX = 0;
	double m_lastY = 0;
	float m_sensitivity = 0.003;
};