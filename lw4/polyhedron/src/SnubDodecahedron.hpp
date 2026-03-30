#pragma once

#include "ObjLoader.hpp"
#include <glad/glad.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>
#include <string>
#include <vector>

class SnubDodecahedron final : public Drawable
{
public:
	explicit SnubDodecahedron(const std::string& objPath)
		: m_mesh(CreateVertices(objPath), GL_TRIANGLES)
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection) override
	{
		glm::mat4 model = GetTransformMatrix();
		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformMat4("uModel", model);

		for (size_t i = 0; i < m_faces.size(); ++i)
		{
			shader.SetUniformVec4("uColor", FaceColor(static_cast<int>(i)));
			m_mesh.Draw(m_faces[i].offset, m_faces[i].size);
		}
	}

private:
	static constexpr GLsizei VertsPerPentagon = 9; // 3 треугольника по 3 вершины

	std::vector<Vertex> CreateVertices(const std::string& objPath)
	{
		ObjData obj = ObjLoader::Load(objPath);

		const auto& pentagons = obj.groups.at("pentagons");
		const auto& triangles = obj.groups.at("triangles");

		std::vector<Vertex> vertices;
		vertices.reserve(pentagons.size() * VertsPerPentagon + triangles.size() * 3);

		AddPentagons(vertices, obj.vertices, pentagons);
		AddTriangles(vertices, obj.vertices, triangles);

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

			m_faces.push_back({ static_cast<GLint>(vertices.size()), VertsPerPentagon });

			AddTriangle(vertices, p0, p1, p2);
			AddTriangle(vertices, p0, p2, p3);
			AddTriangle(vertices, p0, p3, p4);
		}
	}

	void AddTriangles(
		std::vector<Vertex>& vertices,
		const std::vector<glm::vec3>& rawVertices,
		const std::vector<std::vector<int>>& trianglesIndexes)
	{
		for (const std::vector<int>& triangleIndex : trianglesIndexes)
		{
			glm::vec3 p0 = rawVertices[triangleIndex[0]];
			glm::vec3 p1 = rawVertices[triangleIndex[1]];
			glm::vec3 p2 = rawVertices[triangleIndex[2]];
			m_faces.push_back({ static_cast<GLint>(vertices.size()), 3 });

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

	static glm::vec4 FaceColor(int index)
	{
		return { 0.95, 0.05, 0.95, 1 };
	}

	struct Face
	{
		GLint offset, size;
	};

	std::vector<Face> m_faces;
	Mesh m_mesh;
};