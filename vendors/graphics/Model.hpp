#pragma once

#include "Material.hpp"
#include "TexturedMesh.hpp"
#include "shaders/ShaderProgram.hpp"
#include "Texture.hpp"

#include <utility>
#include <vector>

class Model
{
public:
	struct SubMesh
	{
		TexturedMesh mesh;
		int materialIndex;
	};

	Model(std::vector<Material>&& materials, std::vector<SubMesh>&& submeshes)
		: m_materials(std::move(materials))
		, m_submeshes(std::move(submeshes))
	{
	}

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	Model(Model&&) = default;
	Model& operator=(Model&&) = default;

	void Draw(ShaderProgram& shader)
	{
		for (auto& sub : m_submeshes)
		{
			m_materials[sub.materialIndex].Apply(shader);
			sub.mesh.Draw(GL_TRIANGLES);
		}
	}

	void SetDiffuseTexture(const Texture* texture)
	{
		for (auto& mat : m_materials)
		{
			mat.SetDiffuseTexture(texture);
		}
	}

	void SetMaterial(const Material& material)
	{
		for (auto& mat : m_materials)
		{
			mat = material;
		}
	}

private:
	std::vector<Material> m_materials;
	std::vector<SubMesh> m_submeshes;
};
