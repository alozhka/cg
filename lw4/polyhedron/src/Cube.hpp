#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>
#include <vector>

class Cube final : public Drawable
{
public:
	explicit Cube(float halfExtent = 0.5f)
		: m_mesh(CreateVertices(halfExtent), GL_TRIANGLES)
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& viewProjection) override
	{
		glm::mat4 model = GetTransformMatrix();
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

		shader.SetUniformMat4("uViewProjection", viewProjection);
		shader.SetUniformMat4("uModel", model);
		shader.SetUniformMat3("uNormalMatrix", normalMatrix);

		constexpr GLsizei vertsPerFace = 6;
		for (int face = 0; face < 6; ++face)
		{
			shader.SetUniformVec4("uColor", FaceColor(face));
			m_mesh.Draw(face * vertsPerFace, vertsPerFace);
		}
	}

private:
	static std::vector<Vertex> CreateVertices(float h)
	{
		return {
			// Front (z+), normal (0, 0, 1)
			{ { -h, -h, h }, { 0, 0, 1 } }, { { h, -h, h }, { 0, 0, 1 } }, { { h, h, h }, { 0, 0, 1 } },
			{ { -h, -h, h }, { 0, 0, 1 } }, { { h, h, h }, { 0, 0, 1 } }, { { -h, h, h }, { 0, 0, 1 } },
			// Back (z-), normal (0, 0, -1)
			{ { h, -h, -h }, { 0, 0, -1 } }, { { -h, -h, -h }, { 0, 0, -1 } }, { { -h, h, -h }, { 0, 0, -1 } },
			{ { h, -h, -h }, { 0, 0, -1 } }, { { -h, h, -h }, { 0, 0, -1 } }, { { h, h, -h }, { 0, 0, -1 } },
			// Right (x+), normal (1, 0, 0)
			{ { h, -h, h }, { 1, 0, 0 } }, { { h, -h, -h }, { 1, 0, 0 } }, { { h, h, -h }, { 1, 0, 0 } },
			{ { h, -h, h }, { 1, 0, 0 } }, { { h, h, -h }, { 1, 0, 0 } }, { { h, h, h }, { 1, 0, 0 } },
			// Left (x-), normal (-1, 0, 0)
			{ { -h, -h, -h }, { -1, 0, 0 } }, { { -h, -h, h }, { -1, 0, 0 } }, { { -h, h, h }, { -1, 0, 0 } },
			{ { -h, -h, -h }, { -1, 0, 0 } }, { { -h, h, h }, { -1, 0, 0 } }, { { -h, h, -h }, { -1, 0, 0 } },
			// Top (y+), normal (0, 1, 0)
			{ { -h, h, h }, { 0, 1, 0 } }, { { h, h, h }, { 0, 1, 0 } }, { { h, h, -h }, { 0, 1, 0 } },
			{ { -h, h, h }, { 0, 1, 0 } }, { { h, h, -h }, { 0, 1, 0 } }, { { -h, h, -h }, { 0, 1, 0 } },
			// Bottom (y-), normal (0, -1, 0)
			{ { -h, -h, -h }, { 0, -1, 0 } }, { { h, -h, -h }, { 0, -1, 0 } }, { { h, -h, h }, { 0, -1, 0 } },
			{ { -h, -h, -h }, { 0, -1, 0 } }, { { h, -h, h }, { 0, -1, 0 } }, { { -h, -h, h }, { 0, -1, 0 } },
		};
	}

	static glm::vec4 FaceColor(int faceIndex)
	{
		static const glm::vec4 colors[6] = {
			{ 0.85f, 0.25f, 0.2f, 1.f },
			{ 0.2f, 0.75f, 0.9f, 1.f },
			{ 0.35f, 0.85f, 0.35f, 1.f },
			{ 0.85f, 0.35f, 0.85f, 1.f },
			{ 0.3f, 0.45f, 0.95f, 1.f },
			{ 0.95f, 0.85f, 0.25f, 1.f },
		};
		return colors[faceIndex];
	}

	Mesh m_mesh;
};