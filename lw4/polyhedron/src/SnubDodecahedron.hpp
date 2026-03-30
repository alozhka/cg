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
		// Пятиугольники
		for (int i = 0; i < PentagonCount; ++i)
		{
			m_faceOffsets[i] = i * VertsPerPentagon;
			m_faceSizes[i] = VertsPerPentagon;
		}
		// Треугольники
		constexpr GLsizei triStart = PentagonCount * VertsPerPentagon;
		for (int i = 0; i < TriangleCount; ++i)
		{
			m_faceOffsets[PentagonCount + i] = triStart + i * 3;
			m_faceSizes[PentagonCount + i] = 3;
		}
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection) override
	{
		glm::mat4 model = GetTransformMatrix();
		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformMat4("uModel", model);

		for (size_t i = 0; i < m_faceCount; ++i)
		{
			shader.SetUniformVec4("uColor", FaceColor(static_cast<int>(i)));
			m_mesh.Draw(m_faceOffsets[i], m_faceSizes[i]);
		}
	}

private:
	static constexpr int VertexCount = 60;
	static constexpr int PentagonCount = 12;
	static constexpr int TriangleCount = 80;
	static constexpr int TotalFaces = PentagonCount + TriangleCount;
	static constexpr GLsizei VertsPerPentagon = 9; // 3 треугольника по 3 вершины

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
			if (idx < VertexCount)
				V[idx++] = glm::vec3{ vx, vy, vz };
		};

		// V0-V3
		add(c2, c1, c14);
		add(c2, -c1, -c14);
		add(-c2, -c1, c14);
		add(-c2, c1, -c14);
		// V4-V7
		add(c14, c2, c1);
		add(c14, -c2, -c1);
		add(-c14, -c2, c1);
		add(-c14, c2, -c1);
		// V8-V11
		add(c1, c14, c2);
		add(c1, -c14, -c2);
		add(-c1, -c14, c2);
		add(-c1, c14, -c2);
		// V12-V15
		add(c3, -c4, c13);
		add(c3, c4, -c13);
		add(-c3, c4, c13);
		add(-c3, -c4, -c13);
		// V16-V19
		add(c13, -c3, c4);
		add(c13, c3, -c4);
		add(-c13, c3, c4);
		add(-c13, -c3, -c4);
		// V20-V23
		add(c4, -c13, c3);
		add(c4, c13, -c3);
		add(-c4, c13, c3);
		add(-c4, -c13, -c3);
		// V24-V27
		add(c0, c8, c12);
		add(c0, -c8, -c12);
		add(-c0, -c8, c12);
		add(-c0, c8, -c12);
		// V28-V31
		add(c12, c0, c8);
		add(c12, -c0, -c8);
		add(-c12, -c0, c8);
		add(-c12, c0, -c8);
		// V32-V35
		add(c8, c12, c0);
		add(c8, -c12, -c0);
		add(-c8, -c12, c0);
		add(-c8, c12, -c0);
		// V36-V39
		add(c7, c6, c11);
		add(c7, -c6, -c11);
		add(-c7, -c6, c11);
		add(-c7, c6, -c11);
		// V40-V43
		add(c11, c7, c6);
		add(c11, -c7, -c6);
		add(-c11, -c7, c6);
		add(-c11, c7, -c6);
		// V44-V47
		add(c6, c11, c7);
		add(c6, -c11, -c7);
		add(-c6, -c11, c7);
		add(-c6, c11, -c7);
		// V48-V51
		add(c9, -c5, c10);
		add(c9, c5, -c10);
		add(-c9, c5, c10);
		add(-c9, -c5, -c10);
		// V52-V55
		add(c10, -c9, c5);
		add(c10, c9, -c5);
		add(-c10, c9, c5);
		add(-c10, -c9, -c5);
		// V56-V59
		add(c5, -c10, c9);
		add(c5, c10, -c9);
		add(-c5, c10, c9);
		add(-c5, -c10, -c9);

		for (auto& v : V)
			v = glm::normalize(v);

		return V;
	}

	static constexpr std::array<std::array<int, 5>, PentagonCount> Pentagons = { {
		{ { 0, 12, 48, 28, 36 } },
		{ { 1, 13, 49, 29, 37 } },
		{ { 2, 14, 50, 30, 38 } },
		{ { 3, 15, 51, 31, 39 } },
		{ { 4, 17, 53, 32, 40 } },
		{ { 5, 16, 52, 33, 41 } },
		{ { 6, 19, 55, 34, 42 } },
		{ { 7, 18, 54, 35, 43 } },
		{ { 8, 22, 58, 24, 44 } },
		{ { 9, 23, 59, 25, 45 } },
		{ { 10, 20, 56, 26, 46 } },
		{ { 11, 21, 57, 27, 47 } },
	} };

	static constexpr std::array<std::array<int, 3>, TriangleCount> Triangles = { {
		{ { 0, 14, 2 } },
		{ { 1, 15, 3 } },
		{ { 2, 12, 0 } },
		{ { 3, 13, 1 } },
		{ { 4, 16, 5 } },
		{ { 5, 17, 4 } },
		{ { 6, 18, 7 } },
		{ { 7, 19, 6 } },
		{ { 8, 21, 11 } },
		{ { 9, 20, 10 } },
		{ { 10, 23, 9 } },
		{ { 11, 22, 8 } },
		{ { 12, 56, 48 } },
		{ { 13, 57, 49 } },
		{ { 14, 58, 50 } },
		{ { 15, 59, 51 } },
		{ { 16, 48, 52 } },
		{ { 17, 49, 53 } },
		{ { 18, 50, 54 } },
		{ { 19, 51, 55 } },
		{ { 20, 52, 56 } },
		{ { 21, 53, 57 } },
		{ { 22, 54, 58 } },
		{ { 23, 55, 59 } },
		{ { 24, 36, 44 } },
		{ { 25, 37, 45 } },
		{ { 26, 38, 46 } },
		{ { 27, 39, 47 } },
		{ { 28, 40, 36 } },
		{ { 29, 41, 37 } },
		{ { 30, 42, 38 } },
		{ { 31, 43, 39 } },
		{ { 32, 44, 40 } },
		{ { 33, 45, 41 } },
		{ { 34, 46, 42 } },
		{ { 35, 47, 43 } },
		{ { 36, 24, 0 } },
		{ { 37, 25, 1 } },
		{ { 38, 26, 2 } },
		{ { 39, 27, 3 } },
		{ { 40, 28, 4 } },
		{ { 41, 29, 5 } },
		{ { 42, 30, 6 } },
		{ { 43, 31, 7 } },
		{ { 44, 32, 8 } },
		{ { 45, 33, 9 } },
		{ { 46, 34, 10 } },
		{ { 47, 35, 11 } },
		{ { 48, 16, 28 } },
		{ { 49, 17, 29 } },
		{ { 50, 18, 30 } },
		{ { 51, 19, 31 } },
		{ { 52, 20, 33 } },
		{ { 53, 21, 32 } },
		{ { 54, 22, 35 } },
		{ { 55, 23, 34 } },
		{ { 56, 12, 26 } },
		{ { 57, 13, 27 } },
		{ { 58, 14, 24 } },
		{ { 59, 15, 25 } },
		{ { 24, 14, 0 } },
		{ { 25, 15, 1 } },
		{ { 26, 12, 2 } },
		{ { 27, 13, 3 } },
		{ { 28, 16, 4 } },
		{ { 29, 17, 5 } },
		{ { 30, 18, 6 } },
		{ { 31, 19, 7 } },
		{ { 32, 21, 8 } },
		{ { 33, 20, 9 } },
		{ { 34, 23, 10 } },
		{ { 35, 22, 11 } },
		{ { 36, 40, 44 } },
		{ { 37, 41, 45 } },
		{ { 38, 42, 46 } },
		{ { 39, 43, 47 } },
		{ { 48, 56, 52 } },
		{ { 49, 57, 53 } },
		{ { 50, 58, 54 } },
		{ { 51, 59, 55 } },
	} };

	// === Создание меша ===
	static std::vector<Vertex> CreateVertices()
	{
		auto V = GenerateVertices();
		std::vector<Vertex> vertices;
		vertices.reserve(TriangleCount * 3 + PentagonCount * VertsPerPentagon);

		auto addTriangle = [&](int i0, int i1, int i2) {
			glm::vec3 p0 = V[i0], p1 = V[i1], p2 = V[i2];
			glm::vec3 normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));
			glm::vec3 center = (p0 + p1 + p2) * 0.333f;
			if (glm::dot(normal, center) < 0)
			{
				normal = -normal;
				std::swap(p1, p2);
			}
			vertices.push_back({ p0, normal });
			vertices.push_back({ p1, normal });
			vertices.push_back({ p2, normal });
		};

		for (const auto& pent : Pentagons)
		{
			addTriangle(pent[0], pent[1], pent[2]);
			addTriangle(pent[0], pent[2], pent[3]);
			addTriangle(pent[0], pent[3], pent[4]);
		}

		for (const auto& tri : Triangles)
			addTriangle(tri[0], tri[1], tri[2]);

		return vertices;
	}

	static glm::vec4 FaceColor(int index)
	{
		if (index < PentagonCount)
			return { 0.95f, 0.45f, 0.15f, 1.0f }; // пятиугольники — оранжево-красные
		else
			return { 0.15f, 0.75f, 0.95f, 1.0f }; // треугольники — сине-зелёные
	}

	Mesh m_mesh;
	std::array<GLsizei, TotalFaces> m_faceOffsets{};
	std::array<GLsizei, TotalFaces> m_faceSizes{};
	size_t m_faceCount = TotalFaces;
};