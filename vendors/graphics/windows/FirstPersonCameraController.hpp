#pragma once

#include "../camera/FirstPersonCamera.hpp"

class FirstPersonCameraController
{
public:
	explicit FirstPersonCameraController(FirstPersonCamera& camera)
		: m_camera(camera)
	{
	}

	void OnRawMouseMove(double x, double y)
	{
		if (!m_initialized)
		{
			m_lastX = x;
			m_lastY = y;
			m_initialized = true;
			return;
		}

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
	bool m_initialized = false;
	float m_sensitivity = 0.003f;
};