#pragma once

#include "Material.hpp"
#include "Model.hpp"
#include "TexturedMesh.hpp"
#include "TexturedVertex.hpp"
#include "textures/TextureCache.hpp"

#include <tiny_obj_loader.h>

#include <glad/glad.h>

#include <cstddef>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace ObjLoader
{

namespace detail
{

inline tinyobj::ObjReader ParseObj(const std::string& objPath, const std::filesystem::path& baseDir)
{
	tinyobj::ObjReaderConfig config;
	config.mtl_search_path = baseDir.string();
	config.triangulate = true;

	tinyobj::ObjReader reader;
	if (!reader.ParseFromFile(objPath, config))
	{
		throw std::runtime_error("Failed to load OBJ '" + objPath + "': " + reader.Error());
	}
	return reader;
}

inline Material MakeMaterial(
	const tinyobj::material_t& mat,
	const std::filesystem::path& baseDir,
	TextureCache& textureCache)
{
	Material material(
		glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]),
		glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]),
		glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]),
		mat.shininess > 0 ? mat.shininess : 32);

	if (!mat.diffuse_texname.empty())
	{
		const std::filesystem::path texPath = baseDir / mat.diffuse_texname;
		material.SetDiffuseTexture(&textureCache.Load(texPath.string()));
	}

	return material;
}

inline TexturedVertex MakeVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& idx)
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

inline std::vector<Material> BuildMaterials(
	const std::vector<tinyobj::material_t>& objMaterials,
	const std::filesystem::path& baseDir,
	TextureCache& textureCache)
{
	std::vector<Material> materials;
	materials.reserve(objMaterials.size() + 1);
	for (const auto& mat : objMaterials)
	{
		materials.push_back(MakeMaterial(mat, baseDir, textureCache));
	}
	materials.emplace_back();
	return materials;
}

inline int ResolveMaterialId(int matId, int defaultMaterialIndex)
{
	return (matId < 0 || matId >= defaultMaterialIndex) ? defaultMaterialIndex : matId;
}

struct IndexKey
{
	int vertex;
	int normal;
	int texCoord;

	bool operator==(const IndexKey& other) const
	{
		return vertex == other.vertex && normal == other.normal && texCoord == other.texCoord;
	}
};

struct IndexKeyHash
{
	std::size_t operator()(const IndexKey& k) const noexcept
	{
		std::size_t h = std::hash<int>{}(k.vertex);
		h ^= std::hash<int>{}(k.normal) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= std::hash<int>{}(k.texCoord) + 0x9e3779b9 + (h << 6) + (h >> 2);
		return h;
	}
};

struct IndexedBucket
{
	std::vector<TexturedVertex> vertices;
	std::vector<GLuint> indices;
	std::unordered_map<IndexKey, GLuint, IndexKeyHash> dedup;

	void AppendVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& idx)
	{
		const IndexKey key{ idx.vertex_index, idx.normal_index, idx.texcoord_index };
		const auto [it, inserted] = dedup.try_emplace(key, static_cast<GLuint>(vertices.size()));
		if (inserted)
		{
			vertices.push_back(MakeVertex(attrib, idx));
		}
		indices.push_back(it->second);
	}

	bool Empty() const { return indices.empty(); }

	Model::SubMesh IntoSubMesh(int materialIndex) const
	{
		return { TexturedMesh(vertices, indices), materialIndex };
	}
};

inline std::vector<Model::SubMesh> BuildSubmeshes(
	const std::vector<tinyobj::shape_t>& shapes,
	const tinyobj::attrib_t& attrib,
	int materialCount)
{
	const int defaultMaterialIndex = materialCount - 1;
	std::vector<IndexedBucket> buckets(materialCount);

	for (const auto& shape : shapes)
	{
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
		{
			auto& bucket = buckets[ResolveMaterialId(shape.mesh.material_ids[f], defaultMaterialIndex)];
			for (int v = 0; v < 3; ++v)
			{
				bucket.AppendVertex(attrib, shape.mesh.indices[3 * f + v]);
			}
		}
	}

	std::vector<Model::SubMesh> submeshes;
	for (size_t matId = 0; matId < buckets.size(); ++matId)
	{
		IndexedBucket& bucket = buckets[matId];
		if (!bucket.Empty())
		{
			submeshes.push_back(bucket.IntoSubMesh(matId));
		}
	}
	return submeshes;
}

} // namespace detail

inline Model Load(const std::string& objPath, TextureCache& textureCache)
{
	const std::filesystem::path objFile(objPath);
	const tinyobj::ObjReader reader = detail::ParseObj(objPath, objFile.parent_path());

	auto materials = detail::BuildMaterials(reader.GetMaterials(), objFile.parent_path(), textureCache);
	auto submeshes = detail::BuildSubmeshes(reader.GetShapes(), reader.GetAttrib(), materials.size());
	return Model(std::move(materials), std::move(submeshes));
}

} // namespace ObjLoader