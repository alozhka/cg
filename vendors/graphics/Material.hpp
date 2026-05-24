#pragma once

#include "Texture.hpp"
#include "shaders/ShaderProgram.hpp"

#include <glm/vec3.hpp>

class Material
{
public:
	Material() = default;

	Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
		: m_ambient(ambient)
		, m_diffuse(diffuse)
		, m_specular(specular)
		, m_shininess(shininess)
	{
	}

	void SetDiffuseTexture(const Texture* texture)
	{
		m_diffuseTexture = texture;
	}

	void Apply(ShaderProgram& shader) const
	{
		shader.SetUniformVec3("uMaterial.ambient", m_ambient);
		shader.SetUniformVec3("uMaterial.diffuse", m_diffuse);
		shader.SetUniformVec3("uMaterial.specular", m_specular);
		shader.SetUniformFloat("uMaterial.shininess", m_shininess);

		const bool hasTexture = m_diffuseTexture != nullptr;
		shader.SetUniformInt("uMaterial.hasDiffuseTexture", hasTexture ? 1 : 0);
		if (hasTexture)
		{
			m_diffuseTexture->Bind(0);
			shader.SetUniformInt("uMaterial.diffuseTexture", 0);
		}
	}

private:
	glm::vec3 m_ambient{ 0.2, 0.2, 0.2 };
	glm::vec3 m_diffuse{ 0.8, 0.8, 0.8 };
	glm::vec3 m_specular{ 0.5, 0.5, 0.5 };
	float m_shininess = 32;
	const Texture* m_diffuseTexture = nullptr;
};