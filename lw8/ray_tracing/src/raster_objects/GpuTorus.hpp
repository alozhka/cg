#pragma once

#include "../shading/MaterialData.hpp"

#include <graphics/Drawable3D.hpp>
#include <graphics/Mesh.hpp>
#include <graphics/Vertex.hpp>

#include <cmath>
#include <utility>
#include <vector>

class GpuTorus : public Drawable3D
{
public:
	GpuTorus(float majorRadius, float minorRadius, const glm::vec3& position, MaterialPtr material)
		: Drawable3D(position)
		, m_material(std::move(material))
		, m_mesh(BuildMesh(majorRadius, minorRadius))
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		const glm::mat4 modelTransform = parentTransform * GetTransformMatrix();
		shader.SetUniformMat4("uModel", modelTransform);
		shader.SetUniformMat3("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(modelTransform))));
		SetMaterialUniforms(shader);
		m_mesh.Draw(GL_TRIANGLES);
	}

private:
	static constexpr int MAJOR_SEGMENTS = 64;
	static constexpr int MINOR_SEGMENTS = 24;

	static Mesh BuildMesh(float R, float r)
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		vertices.reserve((MAJOR_SEGMENTS + 1) * (MINOR_SEGMENTS + 1));
		indices.reserve(MAJOR_SEGMENTS * MINOR_SEGMENTS * 6);


		for (int i = 0; i <= MAJOR_SEGMENTS; ++i)
		{
			const float u = glm::two_pi<float>() * static_cast<float>(i) / MAJOR_SEGMENTS;
			const float cosU = std::cos(u);
			const float sinU = std::sin(u);

			for (int j = 0; j <= MINOR_SEGMENTS; ++j)
			{
				const float v = glm::two_pi<float>() * static_cast<float>(j) / MINOR_SEGMENTS;
				const float cosV = std::cos(v);
				const float sinV = std::sin(v);

				const glm::vec3 position{
					(R + r * cosV) * cosU,
					r * sinV,
					(R + r * cosV) * sinU,
				};
				const glm::vec3 normal{ cosV * cosU, sinV, cosV * sinU };
				vertices.push_back({ position, normal });
			}
		}

		const int stride = MINOR_SEGMENTS + 1;
		for (int i = 0; i < MAJOR_SEGMENTS; ++i)
		{
			for (int j = 0; j < MINOR_SEGMENTS; ++j)
			{
				const GLuint a = i * stride + j;
				const GLuint b = a + 1;
				const GLuint c = a + stride;
				const GLuint d = c + 1;

				indices.push_back(a);
				indices.push_back(c);
				indices.push_back(b);

				indices.push_back(b);
				indices.push_back(c);
				indices.push_back(d);
			}
		}

		return Mesh(vertices, indices);
	}

	void SetMaterialUniforms(ShaderProgram& shader) const
	{
		if (!m_material)
		{
			return;
		}
		shader.SetUniformVec3("uMaterial.ambient", m_material->ambient);
		shader.SetUniformVec3("uMaterial.diffuse", m_material->diffuse);
		shader.SetUniformVec3("uMaterial.specular", m_material->specular);
		shader.SetUniformFloat("uMaterial.shininess", m_material->shininess);
	}

	MaterialPtr m_material;
	Mesh m_mesh;
};