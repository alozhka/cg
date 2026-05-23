#pragma once

#include "../camera/FirstPersonCamera.hpp"
#include "KeyboardReader.hpp"

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

class KeyboardCameraController
{
public:
	KeyboardCameraController(FirstPersonCamera& camera, KeyboardReader& keys)
		: m_camera(camera)
		, m_keys(keys)
	{
	}

	void Update(float dt)
	{
		const glm::vec3 forward = m_camera.GetForwardXZ();
		const glm::vec3 right = m_camera.GetRight();
		const float step = m_moveSpeed * dt;

		glm::vec3 pos = m_camera.GetPosition();
		if (m_keys.IsButtonPressed(GLFW_KEY_W))
		{
			pos += forward * step;
		}
		if (m_keys.IsButtonPressed(GLFW_KEY_S))
		{
			pos -= forward * step;
		}
		if (m_keys.IsButtonPressed(GLFW_KEY_D))
		{
			pos += right * step;
		}
		if (m_keys.IsButtonPressed(GLFW_KEY_A))
		{
			pos -= right * step;
		}
		if (m_keys.IsButtonPressed(GLFW_KEY_SPACE))
		{
			pos.y += step;
		}
		if (m_keys.IsButtonPressed(GLFW_KEY_LEFT_SHIFT))
		{
			pos.y -= step;
		}
		m_camera.SetPosition(pos);
	}

	void SetMoveSpeed(float speed) { m_moveSpeed = speed; }

private:
	FirstPersonCamera& m_camera;
	KeyboardReader& m_keys;
	float m_moveSpeed = 3.0f;
};