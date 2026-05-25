#pragma once

#include <algorithm>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>

class FirstPersonCamera
{
public:
	void SetPosition(const glm::vec3& pos)
	{
		m_position = pos;
	}

	void AddYaw(float d)
	{
		m_yaw += d;
	}

	void AddPitch(float d)
	{
		m_pitch = std::clamp(m_pitch + d, -m_pitchLimit, m_pitchLimit);
	}

	glm::vec3 GetForward() const
	{
		return glm::normalize(glm::vec3{
			std::cos(m_pitch) * std::cos(m_yaw),
			std::sin(m_pitch),
			std::cos(m_pitch) * std::sin(m_yaw) });
	}

	glm::vec3 GetForwardXZ() const
	{
		return glm::normalize(glm::vec3{ std::cos(m_yaw), 0, std::sin(m_yaw) });
	}

	glm::vec3 GetRightXZ() const
	{
		glm::vec3 fwd = GetForwardXZ();
		return glm::vec3{ fwd.z, 0, -fwd.x };
	}

	glm::vec3 GetRight() const
	{
		glm::vec3 fwd = GetForwardXZ();
		return glm::normalize(glm::vec3{ -fwd.z, 0, fwd.x });
	}

	glm::vec3 GetUp() const
	{
		return glm::normalize(glm::cross(GetRight(), GetForward()));
	}

	glm::vec3 GetPosition() const
	{
		return m_position;
	}

	glm::mat4 GetViewMatrix() const
	{
		glm::vec3 target = m_position + GetForward();
		return glm::lookAt(m_position, target, glm::vec3{ 0, 1, 0 });
	}

private:
	glm::vec3 m_position{ 0, 0, 0 };
	float m_yaw = 0;
	float m_pitch = 0;
	float m_pitchLimit = glm::radians(80.f);
};