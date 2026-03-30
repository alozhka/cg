#pragma once

#include <array>
#include <cmath>
#include <glad/glad.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>
#include <vector>

class SnubDodecahedron final : public Drawable
{
public:
	SnubDodecahedron()
		: m_mesh(CreateVertices(), GL_TRIANGLES)
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection) override
	{
		glm::mat4 model = GetTransformMatrix();
		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformMat4("uModel", model);

		for (size_t i = 0; i < TotalFaces; ++i)
		{
			shader.SetUniformVec4("uColor", FaceColor(static_cast<int>(i)));
			m_mesh.Draw(m_faces[i].offset, m_faces[i].size);
		}
	}

private:
	static constexpr int VertexCount = 60;
	static constexpr int PentagonCount = 12;
	static constexpr int TriangleCount = 80;
	static constexpr int TotalFaces = PentagonCount + TriangleCount;
	static constexpr GLsizei VertsPerPentagon = 9; // 3 треугольника по 3 вершины

	static std::vector<Vertex> CreateVertices()
	{
		std::array<glm::vec3, VertexCount> rawVertices = GenerateVertices();
		std::vector<Vertex> vertices;
		vertices.reserve(PentagonCount * VertsPerPentagon + TriangleCount * 3);

		for (const std::array<int, 5>& pentagon : Pentagons)
		{
			glm::vec3 p0 = rawVertices[pentagon[0]];
			glm::vec3 p1 = rawVertices[pentagon[1]];
			glm::vec3 p2 = rawVertices[pentagon[2]];
			glm::vec3 p3 = rawVertices[pentagon[3]];
			glm::vec3 p4 = rawVertices[pentagon[4]];

			AddTriangle(vertices, p0, p1, p2);
			AddTriangle(vertices, p0, p2, p3);
			AddTriangle(vertices, p0, p3, p4);
		}

		for (const std::array<int, 3>& triangle : Triangles)
		{
			glm::vec3 p0 = rawVertices[triangle[0]];
			glm::vec3 p1 = rawVertices[triangle[1]];
			glm::vec3 p2 = rawVertices[triangle[2]];

			AddTriangle(vertices, p0, p1, p2);
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

	static std::array<glm::vec3, VertexCount> GenerateVertices()
	{
		float c0 = 0.192893711352359022108262546061;
		float c1 = 0.330921024729844230963655269187;
		float c2 = 0.374821658114562295266609516608;
		float c3 = 0.567715369466921317374872062669;
		float c4 = 0.643029605914072573107464141441;
		float c5 = 0.728335176957191477360671629838;
		float c6 = 0.847550046789060797396217956030;
		float c7 = 1.103156835071753772627281146446;
		float c8 = 1.24950378846302719500774109632;
		float c9 = 1.41526541625598211477109001870;
		float c10 = 1.45402422933801541929649491091;
		float c11 = 1.64691794069037444140475745697;
		float c12 = 1.74618644098582634573474528789;
		float c13 = 1.97783896542021867236841272616;
		float c14 = 2.097053835252087992403959052348;

		std::array<glm::vec3, VertexCount> rawVertices{};
		int idx = 0;

		auto add = [&](float vx, float vy, float vz) {
			rawVertices[idx++] = glm::vec3{ vx, vy, vz };
		};

#include "data/SnubDodecahedronVertices.inc"

		for (glm::vec3& v : rawVertices)
		{
			v = glm::normalize(v);
		}

		return rawVertices;
	}

	static constexpr std::array<std::array<int, 5>, PentagonCount> Pentagons = { {
#include "data/SnubDodecahedronPentagons.inc"

	} };

	static constexpr std::array<std::array<int, 3>, TriangleCount> Triangles = { {
#include "data/SnubDodecahedronTriangles.inc"

	} };

	static glm::vec4 FaceColor(int index)
	{
		return { 0.95, 0.05, 0.95, 1 };
	}

	struct Face
	{
		GLint offset, size;
	};

	static std::array<Face, TotalFaces> SetupFacesSizeAndOffset()
	{
		std::array<Face, TotalFaces> faces{};
		for (int i = 0; i < PentagonCount; ++i)
		{
			faces[i].offset = i * 9;
			faces[i].size = 9;
		}

		constexpr int trianglesOffset = PentagonCount * VertsPerPentagon;
		for (int i = 0; i < TriangleCount; ++i)
		{
			faces[PentagonCount + i].offset = trianglesOffset + i * 3;
			faces[PentagonCount + i].size = 3;
		}

		return faces;
	}

	std::array<Face, TotalFaces> m_faces = SetupFacesSizeAndOffset();
	Mesh m_mesh;
};