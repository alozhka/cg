#pragma once

#include <algorithm>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

class OrbitalCamera
{
public:
	void Move(const glm::vec3& delta)
	{
		m_target += delta;
	}

	void SetTarget(const glm::vec3& t)
	{
		m_target = t;
	}

	void AddYaw(float d)
	{
		m_yaw += d;
	}

	void AddPitch(float d)
	{
		m_pitch = std::clamp(m_pitch + d, -m_pitchLimit, m_pitchLimit);
	}

	glm::vec3 GetPosition() const
	{
		float cp = std::cos(m_pitch);
		return m_target + glm::vec3{
			m_radius * cp * std::cos(m_yaw),
			m_radius * std::sin(m_pitch),
			m_radius * cp * std::sin(m_yaw)
		};
	}

	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(GetPosition(), m_target, glm::vec3{ 0, 1, 0 });
	}

private:
	glm::vec3 m_target{ 0, 0, 0 };

	float m_radius = 3.5;
	float m_yaw = 0.;
	float m_pitch = 0;
	float m_pitchLimit = glm::radians(80.f);
};