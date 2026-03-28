#pragma once

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

	void Apply(ShaderProgram& shader, const glm::mat4& view)
	{
		glm::mat3 viewRot(view);
		glm::vec3 dirView = glm::normalize(viewRot * glm::normalize(m_direction));

		shader.SetUniformVec3("uDirectLight.direction", dirView);
		shader.SetUniformVec3("uDirectLight.diffuse", m_diffuse);
		shader.SetUniformVec3("uDirectLight.ambient", m_ambient);
		shader.SetUniformVec3("uDirectLight.specular", m_specular);
	}

private:
	glm::vec3 m_direction;
	glm::vec4 m_diffuse{ 0.8, 0.8, 0.8, 1 };
	glm::vec4 m_ambient{ 0.2, 0.2, 0.2, 1 };
	glm::vec4 m_specular{ 0.5, 0.5, 0.5, 1 };
};