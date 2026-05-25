#pragma once

#include "../shading/MaterialData.hpp"
#include "RayMesh.hpp"

#include <tiny_obj_loader.h>

#include <cstdint>
#include <filesystem>
#include <glm/vec3.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ObjMeshLoader
{
public:
	struct Loaded
	{
		std::shared_ptr<RayMesh> mesh;
		std::vector<MaterialPtr> materials;
	};

	static Loaded Load(const std::string& path)
	{
		const tinyobj::ObjReader reader = Parse(path);
		auto materials = BuildMaterials(reader.GetMaterials());
		auto mesh = BuildMesh(reader);
		return Loaded{ std::move(mesh), std::move(materials) };
	}

private:
	struct IndexKey
	{
		int vertex;
		int normal;

		bool operator==(const IndexKey& o) const
		{
			return vertex == o.vertex && normal == o.normal;
		}
	};

	struct IndexKeyHash
	{
		std::size_t operator()(const IndexKey& k) const noexcept
		{
			std::size_t h = std::hash<int>{}(k.vertex);
			h ^= std::hash<int>{}(k.normal) + 0x9e3779b9 + (h << 6) + (h >> 2);
			return h;
		}
	};

	static tinyobj::ObjReader Parse(const std::string& filepath)
	{
		const std::filesystem::path objFile(filepath);
		const std::filesystem::path baseDir = objFile.parent_path();

		tinyobj::ObjReaderConfig config;
		config.mtl_search_path = baseDir.string();
		config.triangulate = true;

		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(filepath, config))
		{
			throw std::runtime_error("Failed to load OBJ '" + filepath + "': " + reader.Error());
		}

		return reader;
	}

	static std::vector<MaterialPtr> BuildMaterials(const std::vector<tinyobj::material_t>& src)
	{
		std::vector<MaterialPtr> out;
		out.reserve(src.size());
		for (const auto& material : src)
		{
			MaterialData data;
			data.ambient = glm::vec3{ material.ambient[0], material.ambient[1], material.ambient[2] };
			data.diffuse = glm::vec3{ material.diffuse[0], material.diffuse[1], material.diffuse[2] };
			data.specular = glm::vec3{ material.specular[0], material.specular[1], material.specular[2] };
			data.shininess = material.shininess;
			out.push_back(std::make_shared<MaterialData>(data));
		}
		return out;
	}

	static std::shared_ptr<RayMesh> BuildMesh(const tinyobj::ObjReader& reader)
	{
		const tinyobj::attrib_t& attrib = reader.GetAttrib();

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<MeshTriangle> triangles;
		std::unordered_map<IndexKey, std::uint32_t, IndexKeyHash> dedup;

		for (const auto& shape : reader.GetShapes())
		{
			for (std::size_t face = 0; face < shape.mesh.num_face_vertices.size(); ++face)
			{
				triangles.push_back(MakeTriangle(attrib, shape.mesh, face, positions, normals, dedup));
			}
		}

		return std::make_shared<RayMesh>(std::move(positions), std::move(normals), std::move(triangles));
	}

	static MeshTriangle MakeTriangle(
		const tinyobj::attrib_t& attrib,
		const tinyobj::mesh_t& mesh,
		std::size_t face,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& normals,
		std::unordered_map<IndexKey, std::uint32_t, IndexKeyHash>& bucket)
	{
		const std::size_t firstVertex = 3 * face;
		return MeshTriangle{
			AppendVertex(attrib, mesh.indices[firstVertex + 0], positions, normals, bucket),
			AppendVertex(attrib, mesh.indices[firstVertex + 1], positions, normals, bucket),
			AppendVertex(attrib, mesh.indices[firstVertex + 2], positions, normals, bucket),
			mesh.material_ids[face],
		};
	}

	static std::uint32_t AppendVertex(
		const tinyobj::attrib_t& attrib,
		const tinyobj::index_t& idx,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& normals,
		std::unordered_map<IndexKey, std::uint32_t, IndexKeyHash>& dedup)
	{
		const IndexKey key{ idx.vertex_index, idx.normal_index };
		const auto [it, inserted] = dedup.try_emplace(key, positions.size());
		if (!inserted)
		{
			return it->second;
		}

		positions.emplace_back(
			attrib.vertices[3 * idx.vertex_index + 0],
			attrib.vertices[3 * idx.vertex_index + 1],
			attrib.vertices[3 * idx.vertex_index + 2]);

		normals.emplace_back(
			attrib.normals[3 * idx.normal_index + 0],
			attrib.normals[3 * idx.normal_index + 1],
			attrib.normals[3 * idx.normal_index + 2]);

		return it->second;
	}
};