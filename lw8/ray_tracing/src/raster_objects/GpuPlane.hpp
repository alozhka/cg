#pragma once

#include "../shading/MaterialData.hpp"

#include <graphics/Drawable3D.hpp>
#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>

#include <glm/glm.hpp>

#include <utility>
#include <vector>

class GpuPlane : public Drawable3D
{
public:
	GpuPlane(MaterialPtr material)
		: Drawable3D({ 0, 0, 0 })
		, m_material(std::move(material))
		, m_mesh(BuildMesh())
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		const glm::mat4 modelTransform = parentTransform * GetTransformMatrix();
		shader.SetUniformMat4("uModel", modelTransform);
		SetMaterialUniforms(shader);
		m_mesh.Draw(GL_TRIANGLES);
	}

private:
	static Mesh BuildMesh()
	{
		constexpr glm::vec3 normal{ 0, 1, 0 };
		constexpr float extent = 100;

		const std::vector<Vertex> vertices = {
			{ { -extent, 0, -extent }, normal },
			{ { extent, 0, -extent }, normal },
			{ { extent, 0, extent }, normal },
			{ { -extent, 0, -extent }, normal },
			{ { extent, 0, extent }, normal },
			{ { -extent, 0, extent }, normal },
		};
		return Mesh(vertices);
	}

	void SetMaterialUniforms(ShaderProgram& shader)
	{
		if (!m_material)
		{
			return;
		}
		shader.SetUniformVec3("uMaterial.ambient", m_material->ambient);
		shader.SetUniformVec3("uMaterial.diffuse", m_material->diffuse);
		shader.SetUniformVec3("uMaterial.specular", m_material->specular);
		shader.SetUniformFloat("uMaterial.shininess", m_material->shininess);
	}

	MaterialPtr m_material;
	Mesh m_mesh;
};