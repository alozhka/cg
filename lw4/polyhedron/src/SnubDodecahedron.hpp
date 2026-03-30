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
		vertices.reserve(TriangleCount * 3 + PentagonCount * VertsPerPentagon);

		for (const std::array<int, 5>& pentagon : Pentagons)
		{
			AddTriangle(rawVertices, vertices, pentagon[0], pentagon[1], pentagon[2]);
			AddTriangle(rawVertices, vertices, pentagon[0], pentagon[2], pentagon[3]);
			AddTriangle(rawVertices, vertices, pentagon[0], pentagon[3], pentagon[4]);
		}

		for (const std::array<int, 3>& triangle : Triangles)
		{
			AddTriangle(rawVertices, vertices, triangle[0], triangle[1], triangle[2]);
		}

		return vertices;
	}

	static void AddTriangle(
		const std::array<glm::vec3, VertexCount>& rawVertices,
		std::vector<Vertex>& vertices,
		int index0,
		int index1,
		int index2)
	{
		glm::vec3 p0 = rawVertices[index0];
		glm::vec3 p1 = rawVertices[index1];
		glm::vec3 p2 = rawVertices[index2];

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

		std::array<glm::vec3, VertexCount> V{};
		int idx = 0;

		auto add = [&](float vx, float vy, float vz) {
			V[idx++] = glm::vec3{ vx, vy, vz };
		};

#include "data/SnubDodecahedronVertices.inc"

		for (auto& v : V)
			v = glm::normalize(v);

		return V;
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