#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <graphics/shaders/ShaderProgram.hpp>

class PointLight
{
public:
	void SetPosition(const glm::vec3& position)
	{
		m_position = position;
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

	void SetAttenuation(float constant, float linear, float quadratic)
	{
		m_constant = constant;
		m_linear = linear;
		m_quadratic = quadratic;
	}

	void Apply(ShaderProgram& shader, int index)
	{
		std::string prefix = "uPointLights[" + std::to_string(index) + "]";
		shader.SetUniformVec3(prefix + ".position", m_position);
		shader.SetUniformVec3(prefix + ".diffuse", glm::vec3(m_diffuse));
		shader.SetUniformVec3(prefix + ".ambient", glm::vec3(m_ambient));
		shader.SetUniformVec3(prefix + ".specular", glm::vec3(m_specular));
		shader.SetUniformFloat(prefix + ".constant", m_constant);
		shader.SetUniformFloat(prefix + ".linear", m_linear);
		shader.SetUniformFloat(prefix + ".quadratic", m_quadratic);
	}

private:
	glm::vec3 m_position{ 0, 0, 0 };
	glm::vec4 m_diffuse{ 0.8, 0.8, 0.8, 1 };
	glm::vec4 m_ambient{ 0.2, 0.2, 0.2, 1 };
	glm::vec4 m_specular{ 0.5, 0.5, 0.5, 1 };
	float m_constant = 1.0f;
	float m_linear = 0.35f;
	float m_quadratic = 0.44f;
};