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

	// P(u, v) = ( (R + v·hw·cos(u/2))·cos(u),
	//             (R + v·hw·cos(u/2))·sin(u),
	//              v·hw·sin(u/2) )
	glm::vec3 EvaluatePoint(float u, float v) const
	{
		const float hw = m_width * 0.5f;
		const float r = m_radius + v * hw * glm::cos(u / 2);
		return {
			r * glm::cos(u),
			r * glm::sin(u),
			v * hw * glm::sin(u / 2),
		};
	}

	// N = (∂P/∂u) × (∂P/∂v)
	glm::vec3 EvaluateNormal(float u, float v) const
	{
		const float hw = m_width * 0.5f;
		const float cosU = std::cos(u);
		const float sinU = std::sin(u);
		const float cosHalf = std::cos(u * 0.5f);
		const float sinHalf = std::sin(u * 0.5f);
		const float r = m_radius + v * hw * cosHalf;

		// ∂P/∂u = ( -v·hw·1/2·sin(u/2)·cos(u) - r·sin(u),
		//            -v·hw·1/2·sin(u/2)·sin(u) + r·cos(u),
		//             v·hw·1/2·cos(u/2) )
		const glm::vec3 tangentU = {
			-v * hw * 0.5 * sinHalf * cosU - r * sinU,
			-v * hw * 0.5 * sinHalf * sinU + r * cosU,
			v * hw * 0.5 * cosHalf,
		};

		// ∂P/∂v = ( hw·cos(u/2)·cos(u),
		//           hw·cos(u/2)·sin(u),
		//           hw·sin(u/2) )
		const glm::vec3 tangentV = {
			hw * cosHalf * cosU,
			hw * cosHalf * sinU,
			hw * sinHalf,
		};

		return glm::normalize(glm::cross(tangentU, tangentV));
	}

	float m_radius;
	float m_width;
	Mesh m_mesh;
};