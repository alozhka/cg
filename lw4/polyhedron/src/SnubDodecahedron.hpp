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

		for (const std::vector<int>& pentagon : pentagons)
		{
			glm::vec3 p0 = obj.vertices[pentagon[0]];
			glm::vec3 p1 = obj.vertices[pentagon[1]];
			glm::vec3 p2 = obj.vertices[pentagon[2]];
			glm::vec3 p3 = obj.vertices[pentagon[3]];
			glm::vec3 p4 = obj.vertices[pentagon[4]];

			AddTriangle(vertices, p0, p1, p2);
			AddTriangle(vertices, p0, p2, p3);
			AddTriangle(vertices, p0, p3, p4);

			m_faces.push_back({ static_cast<GLint>(vertices.size()) - VertsPerPentagon, VertsPerPentagon });
		}

		for (const std::vector<int>& triangle : triangles)
		{
			glm::vec3 p0 = obj.vertices[triangle[0]];
			glm::vec3 p1 = obj.vertices[triangle[1]];
			glm::vec3 p2 = obj.vertices[triangle[2]];

			AddTriangle(vertices, p0, p1, p2);

			m_faces.push_back({ static_cast<GLint>(vertices.size()) - 3, 3 });
		}

		return vertices;
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