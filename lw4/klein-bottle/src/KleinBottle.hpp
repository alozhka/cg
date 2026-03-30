#pragma once

#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>
#include <vector>

class KleinBottle final : public Drawable
{
public:
	KleinBottle()
		: m_mesh(GenerateVertices(), GL_TRIANGLES)
	{
	}
	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		glm::mat4 model = GetTransformMatrix();

		shader.SetUniformMat4("uViewProjection", parentTransform);
		shader.SetUniformMat4("uModel", model);
		shader.SetUniformVec4("uColor", { 0.2f, 0.55f, 0.9f, 1.0f });
		m_mesh.Draw();
	}

private:
	static constexpr int NU = 100;
	static constexpr int NV = 80;
	static constexpr float EPS = 1e-4f;

	static glm::vec3 EvalPoint(float u, float v)
	{
		const float f = 4.0f * (1.0f - std::cos(u) / 2.0f);
		float x, y, z;

		if (u <= glm::pi<float>())
		{
			x = 6.0f * std::cos(u) * (1.0f + std::sin(u)) + f * std::cos(u) * std::cos(v);
			y = 16.0f * std::sin(u) + f * std::sin(u) * std::cos(v);
			z = f * std::sin(v);
		}
		else
		{
			x = 6.0f * std::cos(u) * (1.0f + std::sin(u)) - f * std::cos(v);
			y = 16.0f * std::sin(u);
			z = f * std::sin(v);
		}

		return glm::vec3(x, y, z);
	}

	static glm::vec3 ComputeNormal(float u, float v)
	{
		glm::vec3 dPdu = (EvalPoint(u + EPS, v) - EvalPoint(u - EPS, v)) / (2.0f * EPS);
		glm::vec3 dPdv = (EvalPoint(u, v + EPS) - EvalPoint(u, v - EPS)) / (2.0f * EPS);
		glm::vec3 n = glm::cross(dPdu, dPdv);
		float len = glm::length(n);
		if (len < 1e-8f)
			return {0.0f, 1.0f, 0.0f};
		return n / len;
	}

	static std::vector<Vertex> GenerateVertices()
	{
		std::vector<Vertex> vertices;
		vertices.reserve(NU * NV * 6);

		const float uStep = glm::two_pi<float>() / static_cast<float>(NU);
		const float vStep = glm::two_pi<float>() / static_cast<float>(NV);

		auto MakeVertex = [](float u, float v) -> Vertex {
			return { EvalPoint(u, v), ComputeNormal(u, v) };
		};

		for (int i = 0; i < NU; ++i)
		{
			float u0 = i * uStep;
			float u1 = (i + 1) * uStep;

			// Avoid numerical issues at the boundary u=π by staying in the same formula region
			if (u0 < glm::pi<float>() && u1 > glm::pi<float>())
				u1 = glm::pi<float>();
			if (u0 == glm::pi<float>())
				u1 = glm::pi<float>() + uStep;

			for (int j = 0; j < NV; ++j)
			{
				float v0 = j * vStep;
				float v1 = ((j + 1) % NV) * vStep;

				Vertex v00 = MakeVertex(u0, v0);
				Vertex v10 = MakeVertex(u1, v0);
				Vertex v01 = MakeVertex(u0, v1);
				Vertex v11 = MakeVertex(u1, v1);

				// Triangle 1: v00, v10, v11
				vertices.push_back(v00);
				vertices.push_back(v10);
				vertices.push_back(v11);

				// Triangle 2: v00, v11, v01
				vertices.push_back(v00);
				vertices.push_back(v11);
				vertices.push_back(v01);
			}
		}

		return vertices;
	}

	Mesh m_mesh;
};
