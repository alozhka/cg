#pragma once

#include "../core/RayMesh.hpp"
#include "../shading/MaterialData.hpp"

#include <graphics/Drawable3D.hpp>
#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <utility>
#include <vector>

class GpuMesh : public Drawable3D
{
public:
	GpuMesh(const std::shared_ptr<RayMesh>& mesh, MaterialPtr material, const glm::vec3& position)
		: Drawable3D(position)
		, m_material(std::move(material))
		, m_mesh(BuildMesh(*mesh))
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		const glm::mat4 modelTransform = parentTransform * GetTransformMatrix();
		shader.SetUniformMat4("uModel", modelTransform);
		shader.SetUniformMat3("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(modelTransform))));
		SetMaterialUniforms(shader);
		m_mesh.Draw(GL_TRIANGLES);
	}

private:
	static Mesh BuildMesh(const RayMesh& mesh)
	{
		const auto& positions = mesh.Positions();
		const auto& normals = mesh.Normals();
		const auto& triangles = mesh.Triangles();

		std::vector<Vertex> vertices(positions.size());
		for (std::size_t i = 0; i < positions.size(); ++i)
		{
			vertices[i] = { positions[i], normals[i] };
		}

		std::vector<GLuint> indices;
		indices.reserve(triangles.size() * 3);
		for (const auto& tri : triangles)
		{
			indices.push_back(tri.i0);
			indices.push_back(tri.i1);
			indices.push_back(tri.i2);
		}

		return Mesh(vertices, indices);
	}

	void SetMaterialUniforms(ShaderProgram& shader) const
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