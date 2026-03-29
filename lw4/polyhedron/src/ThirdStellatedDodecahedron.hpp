#pragma once

#include <array>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>
#include <vector>

class ThirdStellatedDodecahedron final : public Drawable
{
public:
	ThirdStellatedDodecahedron()
		: m_mesh(CreateVertices(), GL_TRIANGLES)
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection) override
	{
		glm::mat4 model = GetTransformMatrix();

		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformMat4("uModel", model);

		for (int face = 0; face < StarCount; ++face)
		{
			shader.SetUniformVec4("uColor", FaceColor(face));
			m_mesh.Draw(face * VertsPerFace, VertsPerFace);
		}
	}

private:
	static constexpr int StarCount = 12;
	static constexpr int VertexCount = 12;
	static constexpr int TrianglesPerStar = 5;
	static constexpr GLsizei VertsPerFace = 3 * TrianglesPerStar;
	inline static const float StelationFactor = 16 - 6 * std::sqrt(5.0f);

	static std::vector<Vertex> CreateVertices()
	{
		const std::array<glm::vec3, VertexCount> plainVertices = IcosahedronVertices();
		const std::array<std::vector<Vertex>, StarCount> starGroups = BuildStarGroups(plainVertices);

		std::vector<Vertex> vertices;
		vertices.reserve(StarCount * TrianglesPerStar * 3);
		for (const std::vector<Vertex>& group : starGroups)
		{
			vertices.insert(vertices.end(), group.begin(), group.end());
		}

		return vertices;
	}

	static std::array<glm::vec3, VertexCount> IcosahedronVertices()
	{
		const float phi = (1 + std::sqrt(5.0f)) / 2;

		std::array<glm::vec3, VertexCount> V = {
			glm::vec3{ -1, phi, 0 }, glm::vec3{ 1, phi, 0 }, glm::vec3{ -1, -phi, 0 }, glm::vec3{ 1, -phi, 0 },
			glm::vec3{ 0, -1, phi }, glm::vec3{ 0, 1, phi }, glm::vec3{ 0, -1, -phi }, glm::vec3{ 0, 1, -phi },
			glm::vec3{ phi, 0, -1 }, glm::vec3{ phi, 0, 1 }, glm::vec3{ -phi, 0, -1 }, glm::vec3{ -phi, 0, 1 }
		};

		for (glm::vec3& v : V)
		{
			v = glm::normalize(v);
		}

		return V;
	}

	static std::array<std::vector<Vertex>, StarCount> BuildStarGroups(const std::array<glm::vec3, VertexCount>& V)
	{
		// Для каждой из 12 звёзд — 5 граней икосаэдра вокруг вершины j.
		// Первый индекс в каждой тройке — сама вершина j (острие звезды не входит в треугольник,
		// только используется для вычисления centroid и tip).
		constexpr std::array<std::array<std::array<int, 3>, TrianglesPerStar>, StarCount> starFaces = {{
			{{ {0,11,5}, {0,5,1},  {0,1,7},  {0,7,10},  {0,10,11} }}, // star 0
			{{ {1,0,5},  {1,7,0},  {1,5,9},  {1,8,7},   {1,9,8}   }}, // star 1
			{{ {2,11,10},{2,3,4},  {2,6,3},  {2,4,11},  {2,10,6}  }}, // star 2
			{{ {3,9,4},  {3,4,2},  {3,2,6},  {3,6,8},   {3,8,9}   }}, // star 3
			{{ {4,5,11}, {4,3,9},  {4,2,3},  {4,9,5},   {4,11,2}  }}, // star 4
			{{ {5,0,11}, {5,1,0},  {5,9,1},  {5,11,4},  {5,4,9}   }}, // star 5
			{{ {6,10,7}, {6,3,2},  {6,8,3},  {6,2,10},  {6,7,8}   }}, // star 6
			{{ {7,0,1},  {7,10,0}, {7,6,10}, {7,1,8},   {7,8,6}   }}, // star 7
			{{ {8,7,1},  {8,3,6},  {8,9,3},  {8,6,7},   {8,1,9}   }}, // star 8
			{{ {9,1,5},  {9,3,4},  {9,3,8},  {9,4,5},   {9,8,1}   }}, // star 9
			{{ {10,0,7}, {10,11,0},{10,2,11},{10,7,6},  {10,6,2}  }}, // star 10
			{{ {11,5,0}, {11,0,10},{11,4,5}, {11,10,2}, {11,2,4}  }}, // star 11
		}};

		std::array<std::vector<Vertex>, StarCount> starGroups;

		for (int j = 0; j < StarCount; ++j)
		{
			for (const auto& face : starFaces[j])
			{
				glm::vec3 starVertex = V[face[0]]; // = V[j]
				glm::vec3 p1 = V[face[1]];
				glm::vec3 p2 = V[face[2]];

				glm::vec3 center = (starVertex + p1 + p2) / 3.0f;
				glm::vec3 tip = center * StelationFactor;

				glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, tip - p1));

				if (glm::dot(normal, center) < 0)
				{
					normal = -normal;
					std::swap(p1, p2); // Сохраняем обход CCW
				}

				starGroups[j].push_back({ p1, normal });
				starGroups[j].push_back({ p2, normal });
				starGroups[j].push_back({ tip, normal });
			}
		}

		return starGroups;
	}

	static glm::vec4 FaceColor(int faceIndex)
	{
		static const glm::vec4 colors[StarCount] = {
			{ 0.05f, 0.95f, 0.50f, 1 },
			{ 0.05f, 0.95f, 0.50f, 1 },
			{ 0.95f, 0.50f, 0.05f, 1 },
			{ 0.95f, 0.50f, 0.05f, 1 },
			{ 0.95f, 0.95f, 0.05f, 1 },
			{ 0.95f, 0.95f, 0.05f, 1 },
			{ 0.05f, 0.95f, 0.05f, 1 },
			{ 0.05f, 0.95f, 0.05f, 1 },
			{ 0.05f, 0.95f, 0.95f, 1 },
			{ 0.05f, 0.95f, 0.95f, 1 },
			{ 0.95f, 0.05f, 0.95f, 1 },
			{ 0.95f, 0.05f, 0.95f, 1 },
		};
		return colors[faceIndex];
	}

	Mesh m_mesh;
};