#pragma once
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>

class MobiusStrip final : public Drawable
{
public:
	explicit MobiusStrip(float radius = 2, float width = 0.5)
		: m_radius(radius)
		, m_width(width)
		, m_mesh(CreateVertices())
	{
	}
	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		glm::mat4 model = GetTransformMatrix();

		shader.SetUniformMat4("uViewProjection", parentTransform);
		shader.SetUniformMat4("uModel", model);
		shader.SetUniformVec4("uColor", { 0.2, 0.55, 0.9, 1 });
		m_mesh.Draw(GL_TRIANGLES);
	}

private:
	static constexpr float NU = 100;
	static constexpr float NV = 80;
	static constexpr float U_STEP = glm::two_pi<float>() / NU;
	static constexpr float V_STEP = 2 / NV;

	std::vector<Vertex> CreateVertices() const
	{
		std::vector<Vertex> vertices;
		vertices.reserve(NU * NV * 6);

		for (int i = 0; i < NU; ++i)
		{
			float u0 = i * U_STEP;
			float u1 = u0 + U_STEP;

			for (int j = 0; j < NV; ++j)
			{
				float v0 = -1 + j * V_STEP;
				float v1 = v0 + V_STEP;

				Vertex v00 = CreateVertex(u0, v0);
				Vertex v10 = CreateVertex(u1, v0);
				Vertex v01 = CreateVertex(u0, v1);
				Vertex v11 = CreateVertex(u1, v1);

				vertices.insert(vertices.end(), { v00, v10, v11, v00, v01, v11 });
			}
		}

		return vertices;
	}

	Vertex CreateVertex(float u, float v) const
	{
		return { EvaluatePoint(u, v), EvaluateNormal(u, v) };
	}

	glm::vec3 EvaluatePoint(float u, float v) const
	{
		float radialOffset = v * m_width * 0.5f * glm::cos(u / 2);
		float r = m_radius + radialOffset;

		return {
			r * glm::cos(u),
			r * glm::sin(u),
			v * m_width * 0.5 * glm::sin(u / 2),
		};
	}

	glm::vec3 EvaluateNormal(float u, float v) const
	{
		const float halfU = u * 0.5f;
		const float cosU = std::cos(u);
		const float sinU = std::sin(u);
		const float cosHalf = std::cos(halfU);
		const float sinHalf = std::sin(halfU);

		// ∂P/∂u: производная по углу (вдоль ленты)
		const float dr_du = -v * m_width * 0.25f * sinHalf;
		const float dPdu_x = dr_du * cosU - (m_radius + v * m_width * 0.5f * cosHalf) * sinU;
		const float dPdu_y = dr_du * sinU + (m_radius + v * m_width * 0.5f * cosHalf) * cosU;
		const float dPdu_z = v * m_width * 0.25f * cosHalf;

		// ∂P/∂v: производная по ширине (поперёк ленты)
		const float dPdv_x = m_width * 0.5f * cosHalf * cosU;
		const float dPdv_y = m_width * 0.5f * cosHalf * sinU;
		const float dPdv_z = m_width * 0.5f * sinHalf;

		glm::vec3 tangentU(dPdu_x, dPdu_y, dPdu_z);
		glm::vec3 tangentV(dPdv_x, dPdv_y, dPdv_z);

		glm::vec3 normal = glm::cross(tangentU, tangentV);
		const float len = glm::length(normal);

		return normal / len;
	}

	float m_radius;
	float m_width;
	Mesh m_mesh;
};