#pragma once

#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
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

		// У нас 12 звёзд. Каждая звезда состоит из 5 треугольников (15 вершин)
		constexpr GLsizei vertsPerFace = 15;
		for (int face = 0; face < 12; ++face)
		{
			shader.SetUniformVec4("uColor", FaceColor(face));
			m_mesh.Draw(face * vertsPerFace, vertsPerFace);
		}
	}

private:
	static std::vector<Vertex> CreateVertices()
	{
		const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;

		// 12 вершин базового (внутреннего) икосаэдра
		glm::vec3 V[12] = {
			{ -1, phi, 0 }, { 1, phi, 0 }, { -1, -phi, 0 }, { 1, -phi, 0 },
			{ 0, -1, phi }, { 0, 1, phi }, { 0, -1, -phi }, { 0, 1, -phi },
			{ phi, 0, -1 }, { phi, 0, 1 }, { -phi, 0, -1 }, { -phi, 0, 1 }
		};

		for (int i = 0; i < 12; ++i)
		{
			V[i] = glm::normalize(V[i]);
		}

		int faces[20][3] = {
			{ 0, 11, 5 }, { 0, 5, 1 }, { 0, 1, 7 }, { 0, 7, 10 }, { 0, 10, 11 },
			{ 1, 5, 9 }, { 5, 11, 4 }, { 11, 10, 2 }, { 10, 7, 6 }, { 7, 1, 8 },
			{ 3, 9, 4 }, { 3, 4, 2 }, { 3, 2, 6 }, { 3, 6, 8 }, { 3, 8, 9 },
			{ 4, 9, 5 }, { 2, 4, 11 }, { 6, 2, 10 }, { 8, 6, 7 }, { 9, 8, 1 }
		};

		// Временные корзины для 12 плоских звёзд
		std::vector<Vertex> starTriangles[12];

		// Магическая константа: точный коэффициент для Большого звёздчатого додекаэдра
		const float stellationFactor = 15.0f - 6.0f * std::sqrt(5.0f) + 1;

		for (int f = 0; f < 20; ++f)
		{
			glm::vec3 A = V[faces[f][0]];
			glm::vec3 B = V[faces[f][1]];
			glm::vec3 C = V[faces[f][2]];

			glm::vec3 center = (A + B + C) / 3.0f;
			glm::vec3 tip = center * stellationFactor;
			glm::vec3 base[3] = { A, B, C };

			for (int i = 0; i < 3; ++i)
			{
				glm::vec3 p1 = base[i];
				glm::vec3 p2 = base[(i + 1) % 3];

				glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, tip - p1));

				if (glm::dot(normal, center) < 0)
				{
					normal = -normal;
					std::swap(p1, p2); // Сохраняем обход CCW
				}

				// Группировка: находим, к какой из 12 граней-звёзд принадлежит этот треугольник.
				// У Большого звёздчатого додекаэдра нормали 12 граней точно совпадают с векторами вершин икосаэдра.
				int starIndex = 0;
				float maxDot = -1.0f;
				for (int j = 0; j < 12; ++j)
				{
					float d = std::abs(glm::dot(normal, V[j]));
					if (d > maxDot)
					{
						maxDot = d;
						starIndex = j;
					}
				}

				starTriangles[starIndex].push_back({ p1, normal });
				starTriangles[starIndex].push_back({ p2, normal });
				starTriangles[starIndex].push_back({ tip, normal });
			}
		}

		// Сливаем все звёзды в единый плотный буфер
		std::vector<Vertex> vertices;
		vertices.reserve(12 * 5 * 3);
		for (std::vector<Vertex>& starTriangle : starTriangles)
		{
			vertices.insert(vertices.end(), starTriangle.begin(), starTriangle.end());
		}

		return vertices;
	}

	static glm::vec4 FaceColor(int faceIndex)
	{
		// 12 оттенков HSL(h, 0.9, 0.5) с шагом 30° — одинаковая яркость, разные цвета
		static const glm::vec4 colors[12] = {
			{ 0.95f, 0.05f, 0.05f, 1 }, // 0°   красный
			{ 0.95f, 0.50f, 0.05f, 1 }, // 30°  оранжевый
			{ 0.95f, 0.95f, 0.05f, 1 }, // 60°  жёлтый
			{ 0.50f, 0.95f, 0.05f, 1 }, // 90°  жёлто-зелёный
			{ 0.05f, 0.95f, 0.05f, 1 }, // 120° зелёный
			{ 0.05f, 0.95f, 0.50f, 1 }, // 150° зелено-голубой
			{ 0.05f, 0.95f, 0.95f, 1 }, // 180° голубой
			{ 0.05f, 0.50f, 0.95f, 1 }, // 210° лазурный
			{ 0.05f, 0.05f, 0.95f, 1 }, // 240° синий
			{ 0.50f, 0.05f, 0.95f, 1 }, // 270° фиолетовый
			{ 0.95f, 0.05f, 0.95f, 1 }, // 300° пурпурный
			{ 0.95f, 0.05f, 0.50f, 1 }, // 330° малиновый
		};
		return colors[faceIndex];
	}

	Mesh m_mesh;
};