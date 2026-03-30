#pragma once

#include "ObjLoader.hpp"

#include <algorithm>
#include <array>
#include <glad/glad.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>
#include <map>
#include <string>
#include <vector>

class SnubDodecahedron final : public Drawable
{
public:
	explicit SnubDodecahedron(const std::string& objPath)
		: m_mesh(CreateVertices(objPath))
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection) override
	{
		glm::mat4 model = GetTransformMatrix();
		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformMat4("uModel", model);

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1, 1);
		for (const Face& face : m_faces)
		{
			shader.SetUniformVec4("uColor", FaceColor(face.colorIndex));
			m_mesh.Draw(face.drawMode, face.offset, face.size);
		}
		glDisable(GL_POLYGON_OFFSET_FILL);

		shader.SetUniformVec4("uColor", { 0, 0, 0, 1 });
		for (const Face& face : m_faces)
		{
			m_mesh.Draw(GL_LINE_LOOP, face.offset, face.size);
		}
	}

private:
	static constexpr GLsizei VertsPerPentagon = 5; // вершины веера для GL_TRIANGLE_FAN

	std::vector<Vertex> CreateVertices(const std::string& objPath)
	{
		ObjData obj = ObjLoader::Load(objPath);

		const auto& pentagons = obj.groups.at("pentagons");
		const auto& triangles = obj.groups.at("triangles");

		std::vector<Vertex> vertices;
		vertices.reserve(pentagons.size() * VertsPerPentagon + triangles.size() * 3);

		AddPentagons(vertices, obj.vertices, pentagons);
		AddTriangles(vertices, obj.vertices, triangles);

		ComputeColors(pentagons, triangles);

		return vertices;
	}

	void AddPentagons(
		std::vector<Vertex>& vertices,
		const std::vector<glm::vec3>& rawVertices,
		const std::vector<std::vector<int>>& pentagonsIndexes)
	{
		for (const std::vector<int>& pentagonIndexes : pentagonsIndexes)
		{
			glm::vec3 p0 = rawVertices[pentagonIndexes[0]];
			glm::vec3 p1 = rawVertices[pentagonIndexes[1]];
			glm::vec3 p2 = rawVertices[pentagonIndexes[2]];
			glm::vec3 p3 = rawVertices[pentagonIndexes[3]];
			glm::vec3 p4 = rawVertices[pentagonIndexes[4]];

			glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
			m_faces.push_back({ static_cast<GLint>(vertices.size()), VertsPerPentagon, GL_TRIANGLE_FAN, -1 });

			vertices.push_back({ p0, normal });
			vertices.push_back({ p1, normal });
			vertices.push_back({ p2, normal });
			vertices.push_back({ p3, normal });
			vertices.push_back({ p4, normal });
		}
	}

	void AddTriangles(
		std::vector<Vertex>& vertices,
		const std::vector<glm::vec3>& rawVertices,
		const std::vector<std::vector<int>>& trianglesIndexes)
	{
		for (const std::vector<int>& triangleIndexes : trianglesIndexes)
		{
			glm::vec3 p0 = rawVertices[triangleIndexes[0]];
			glm::vec3 p1 = rawVertices[triangleIndexes[1]];
			glm::vec3 p2 = rawVertices[triangleIndexes[2]];
			m_faces.push_back({ static_cast<GLint>(vertices.size()), 3, GL_TRIANGLES, -1 });

			AddTriangle(vertices, p0, p1, p2);
		}
	}

	static void AddTriangle(
		std::vector<Vertex>& vertices,
		glm::vec3 p0,
		glm::vec3 p1,
		glm::vec3 p2)
	{
		glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

		vertices.push_back({ p0, normal });
		vertices.push_back({ p1, normal });
		vertices.push_back({ p2, normal });
	}

	void ComputeColors(
		const std::vector<std::vector<int>>& pentagons,
		const std::vector<std::vector<int>>& triangles)
	{
		std::vector<std::vector<int>> faceIndices;
		faceIndices.insert(faceIndices.end(), pentagons.begin(), pentagons.end());
		faceIndices.insert(faceIndices.end(), triangles.begin(), triangles.end());

		const std::vector<std::vector<int>> adj = BuildAdjacency(faceIndices);
		ApplyGreedyColoring(adj);
	}

	static std::vector<std::vector<int>> BuildAdjacency(
		const std::vector<std::vector<int>>& facesIndices)
	{
		size_t n = facesIndices.size();
		std::map<std::pair<int, int>, int> edgeToFace;
		std::vector<std::vector<int>> adj(n);

		for (size_t currentFaceIndex = 0; currentFaceIndex < n; ++currentFaceIndex)
		{
			const std::vector<int>& faceIndexes = facesIndices[currentFaceIndex];
			for (size_t k = 0; k < faceIndexes.size(); ++k)
			{
				auto edge = std::minmax(faceIndexes[k], faceIndexes[(k + 1) % faceIndexes.size()]);
				auto [it, inserted] = edgeToFace.emplace(edge, currentFaceIndex);
				if (!inserted)
				{
					int firstFaceIndex = it->second;
					adj[currentFaceIndex].push_back(firstFaceIndex);
					adj[firstFaceIndex].push_back(currentFaceIndex);
				}
			}
		}

		return adj;
	}

	void ApplyGreedyColoring(const std::vector<std::vector<int>>& adj)
	{
		for (size_t i = 0; i < m_faces.size(); ++i)
		{
			std::vector<bool> usedColors(3, false);
			for (int neighbor : adj[i])
			{
				if (HasColor(neighbor))
				{
					usedColors[m_faces[neighbor].colorIndex] = true;
				}
			}
			m_faces[i].colorIndex = std::ranges::find(usedColors, false) - usedColors.begin();
		}
	}

	bool HasColor(int faceIndex) const
	{
		return m_faces[faceIndex].colorIndex >= 0;
	}

	static glm::vec4 FaceColor(int colorIndex)
	{
		static constexpr std::array<glm::vec4, 3> palette = {
			glm::vec4{ 0.85, 0.20, 0.20, 1 },
			glm::vec4{ 0.20, 0.70, 0.25, 1 },
			glm::vec4{ 0.20, 0.40, 0.90, 1 },
		};

		if (colorIndex > palette.size() - 1)
		{
			throw std::invalid_argument("Unsupported color index");
		}

		return palette[colorIndex];
	}

	struct Face
	{
		GLint offset, size;
		GLenum drawMode;
		int colorIndex;
	};

	std::vector<Face> m_faces;
	Mesh m_mesh;
};