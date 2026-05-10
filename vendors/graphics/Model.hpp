#pragma once

#include "Material.hpp"
#include "TexturedMesh.hpp"
#include "shaders/ShaderProgram.hpp"
#include "textures/TextureCache.hpp"

#include <filesystem>
#include <tiny_obj_loader.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class Model
{
public:
	Model(const std::string& objPath, TextureCache& textureCache)
	{
		const std::filesystem::path objFile(objPath);

		tinyobj::ObjReaderConfig config;
		config.mtl_search_path = objFile.parent_path().string();
		config.triangulate = true;

		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(objPath, config))
		{
			throw std::runtime_error("Failed to load OBJ '" + objPath + "': " + reader.Error());
		}

		const auto& attrib = reader.GetAttrib();
		const auto& shapes = reader.GetShapes();
		const auto& objMaterials = reader.GetMaterials();

		BuildMaterials(objMaterials, objFile.parent_path(), textureCache);
		const int defaultMaterialIndex = static_cast<int>(m_materials.size()) - 1;

		for (const auto& shape : shapes)
		{
			BuildShapeSubmeshes(shape, attrib, defaultMaterialIndex);
		}
	}

	void Draw(ShaderProgram& shader)
	{
		for (auto& sub : m_submeshes)
		{
			m_materials[sub.materialIndex].Apply(shader);
			sub.mesh->Draw(GL_TRIANGLES);
		}
	}

	void SetDiffuseTexture(const Texture* texture)
	{
		for (auto& mat : m_materials)
		{
			mat.SetDiffuseTexture(texture);
		}
	}

private:
	struct SubMesh
	{
		std::unique_ptr<TexturedMesh> mesh;
		int materialIndex;
	};

	void BuildMaterials(
		const std::vector<tinyobj::material_t>& objMaterials,
		const std::filesystem::path& baseDir,
		TextureCache& textureCache)
	{
		m_materials.reserve(objMaterials.size() + 1);
		for (const auto& mat : objMaterials)
		{
			Material material(
				glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]),
				glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]),
				glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]),
				mat.shininess > 0.0f ? mat.shininess : 32.0f);

			if (!mat.diffuse_texname.empty())
			{
				const std::filesystem::path texPath = baseDir / mat.diffuse_texname;
				const Texture& tex = textureCache.Load(texPath.string());
				material.SetDiffuseTexture(&tex);
			}
			m_materials.push_back(material);
		}
		m_materials.emplace_back();
	}

	void BuildShapeSubmeshes(
		const tinyobj::shape_t& shape,
		const tinyobj::attrib_t& attrib,
		int defaultMaterialIndex)
	{
		std::unordered_map<int, std::vector<TexturedVertex>> verticesByMaterial;

		const auto& mesh = shape.mesh;
		const int materialCount = defaultMaterialIndex;
		size_t indexOffset = 0;
		for (size_t f = 0; f < mesh.num_face_vertices.size(); ++f)
		{
			const int fv = mesh.num_face_vertices[f];
			if (fv != 3)
			{
				indexOffset += fv;
				continue;
			}

			int matId = mesh.material_ids[f];
			if (matId < 0 || matId >= materialCount)
			{
				matId = defaultMaterialIndex;
			}

			auto& bucket = verticesByMaterial[matId];
			for (int v = 0; v < 3; ++v)
			{
				bucket.push_back(MakeVertex(attrib, mesh.indices[indexOffset + v]));
			}
			indexOffset += 3;
		}

		for (auto& [matId, verts] : verticesByMaterial)
		{
			m_submeshes.push_back({
				std::make_unique<TexturedMesh>(verts),
				matId,
			});
		}
	}

	static TexturedVertex MakeVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& idx)
	{
		TexturedVertex vert{};
		vert.position = {
			attrib.vertices[3 * idx.vertex_index + 0],
			attrib.vertices[3 * idx.vertex_index + 1],
			attrib.vertices[3 * idx.vertex_index + 2],
		};
		if (idx.normal_index >= 0)
		{
			vert.normal = {
				attrib.normals[3 * idx.normal_index + 0],
				attrib.normals[3 * idx.normal_index + 1],
				attrib.normals[3 * idx.normal_index + 2],
			};
		}
		if (idx.texcoord_index >= 0)
		{
			vert.texCoord = {
				attrib.texcoords[2 * idx.texcoord_index + 0],
				attrib.texcoords[2 * idx.texcoord_index + 1],
			};
		}
		return vert;
	}

	std::vector<Material> m_materials;
	std::vector<SubMesh> m_submeshes;
};