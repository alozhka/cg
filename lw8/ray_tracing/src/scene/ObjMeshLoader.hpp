#pragma once

#include "RayMesh.hpp"
#include "../shading/MaterialData.hpp"

#include <tiny_obj_loader.h>

#include <cstdint>
#include <filesystem>
#include <glm/vec2.hpp>
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
		const std::filesystem::path objFile(path);
		const std::filesystem::path baseDir = objFile.parent_path();

		tinyobj::ObjReaderConfig config;
		config.mtl_search_path = baseDir.string();
		config.triangulate = true;

		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(path, config))
		{
			throw std::runtime_error("Failed to load OBJ '" + path + "': " + reader.Error());
		}

		const tinyobj::attrib_t& attrib = reader.GetAttrib();
		const auto& shapes = reader.GetShapes();

		auto materials = BuildMaterials(reader.GetMaterials());
		const int defaultIndex = static_cast<int>(materials.size()) - 1;

		const bool fileHasNormals = !attrib.normals.empty();
		const bool fileHasUVs = !attrib.texcoords.empty();

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<MeshTriangle> triangles;
		std::unordered_map<IndexKey, std::uint32_t, IndexKeyHash> dedup;

		for (const auto& shape : shapes)
		{
			const auto& indices = shape.mesh.indices;
			for (std::size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
			{
				MeshTriangle tri{};
				tri.materialIndex = ResolveMaterialId(shape.mesh.material_ids[f], defaultIndex);
				std::uint32_t* slots[3] = { &tri.i0, &tri.i1, &tri.i2 };
				for (int v = 0; v < 3; ++v)
				{
					*slots[v] = AppendVertex(
						attrib, indices[3 * f + v], fileHasNormals, fileHasUVs,
						positions, normals, uvs, dedup);
				}
				triangles.push_back(tri);
			}
		}

		// Если нормалей не было — считаем плоские нормали и пишем их по вершинам.
		// Сглаживание потеряем, но геометрия закрасится.
		if (!fileHasNormals)
		{
			normals.assign(positions.size(), glm::vec3{ 0.f });
			for (const auto& tri : triangles)
			{
				const glm::vec3 e1 = positions[tri.i1] - positions[tri.i0];
				const glm::vec3 e2 = positions[tri.i2] - positions[tri.i0];
				const glm::vec3 n = glm::normalize(glm::cross(e1, e2));
				normals[tri.i0] = n;
				normals[tri.i1] = n;
				normals[tri.i2] = n;
			}
		}

		auto mesh = std::make_shared<RayMesh>(
			std::move(positions), std::move(normals), std::move(uvs), std::move(triangles));
		return Loaded{ std::move(mesh), std::move(materials) };
	}

private:
	struct IndexKey
	{
		int vertex;
		int normal;
		int texCoord;

		bool operator==(const IndexKey& o) const
		{
			return vertex == o.vertex && normal == o.normal && texCoord == o.texCoord;
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

	static std::vector<MaterialPtr> BuildMaterials(const std::vector<tinyobj::material_t>& src)
	{
		std::vector<MaterialPtr> out;
		out.reserve(src.size() + 1);
		for (const auto& m : src)
		{
			MaterialData data;
			data.ambient = glm::vec3{ m.ambient[0], m.ambient[1], m.ambient[2] };
			data.diffuse = glm::vec3{ m.diffuse[0], m.diffuse[1], m.diffuse[2] };
			data.specular = glm::vec3{ m.specular[0], m.specular[1], m.specular[2] };
			data.shininess = m.shininess > 0 ? m.shininess : 32.f;
			out.push_back(std::make_shared<MaterialData>(data));
		}
		// Default-материал в конец — для шейпов без MTL или с material_id < 0.
		out.push_back(std::make_shared<MaterialData>(MaterialData{}));
		return out;
	}

	static int ResolveMaterialId(int matId, int defaultIndex)
	{
		return (matId < 0 || matId >= defaultIndex) ? defaultIndex : matId;
	}

	static std::uint32_t AppendVertex(
		const tinyobj::attrib_t& attrib,
		const tinyobj::index_t& idx,
		bool useNormals,
		bool useUVs,
		std::vector<glm::vec3>& positions,
		std::vector<glm::vec3>& normals,
		std::vector<glm::vec2>& uvs,
		std::unordered_map<IndexKey, std::uint32_t, IndexKeyHash>& dedup)
	{
		const IndexKey key{
			idx.vertex_index,
			useNormals ? idx.normal_index : -1,
			useUVs ? idx.texcoord_index : -1
		};
		const auto [it, inserted] = dedup.try_emplace(key, static_cast<std::uint32_t>(positions.size()));
		if (!inserted)
		{
			return it->second;
		}

		positions.emplace_back(
			attrib.vertices[3 * idx.vertex_index + 0],
			attrib.vertices[3 * idx.vertex_index + 1],
			attrib.vertices[3 * idx.vertex_index + 2]);

		if (useNormals && idx.normal_index >= 0)
		{
			normals.emplace_back(
				attrib.normals[3 * idx.normal_index + 0],
				attrib.normals[3 * idx.normal_index + 1],
				attrib.normals[3 * idx.normal_index + 2]);
		}

		if (useUVs && idx.texcoord_index >= 0)
		{
			uvs.emplace_back(
				attrib.texcoords[2 * idx.texcoord_index + 0],
				attrib.texcoords[2 * idx.texcoord_index + 1]);
		}

		return it->second;
	}
};