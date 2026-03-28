#pragma once
#include "glad/glad.h"

#include <array>
#include <glm/vec3.hpp>

class DirectLight
{
public:
	explicit DirectLight(const glm::vec3& lightDirection)
		: m_direction(lightDirection)
	{
	}

	void SetDirection(const glm::vec3& direction)
	{
		m_direction = direction;
	}

	void SetDiffuseIntensity(float r, float g, float b, float a = 1)
	{
		m_diffuse[0] = r;
		m_diffuse[1] = g;
		m_diffuse[2] = b;
		m_diffuse[3] = a;
	}

	void SetAmbientIntensity(float r, float g, float b, float a = 1)
	{
		m_ambient[0] = r;
		m_ambient[1] = g;
		m_ambient[2] = b;
		m_ambient[3] = a;
	}

	void SetSpecularIntensity(float r, float g, float b, float a = 1)
	{
		m_specular[0] = r;
		m_specular[1] = g;
		m_specular[2] = b;
		m_specular[3] = a;
	}

	void SetLight(GLenum light)
	{
		float lightDirection[4] = { m_direction.x, m_direction.y, m_direction.z, 0 };

		glLightfv(light, GL_POSITION, lightDirection);
		glLightfv(light, GL_DIFFUSE, m_diffuse.data());
		glLightfv(light, GL_AMBIENT, m_ambient.data());
		glLightfv(light, GL_SPECULAR, m_specular.data());
	}

private:
	glm::vec3 m_direction;
	std::array<float, 4> m_diffuse{ 0.8, 0.8, 0.8, 1 };
	std::array<float, 4> m_ambient{ 0.2, 0.2, 0.2, 1 };
	std::array<float, 4> m_specular{ 0.5, 0.5, 0.5, 1 };
};