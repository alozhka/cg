#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <graphics/Drawable.hpp>
#include <graphics/Mesh.hpp>
#include <vector>

class Cube final : public Drawable
{
public:
	explicit Cube(float halfExtent = 0.5f)
		: m_mesh(CreateVertices(halfExtent), GL_TRIANGLES, 3)
	{
	}

	void Draw(ShaderProgram& shader, const glm::mat4& parentTransform) override
	{
		const glm::mat4 mvp = parentTransform * GetTransformMatrix();
		shader.SetUniformMat4("uMVP", mvp);

		constexpr GLsizei vertsPerFace = 6;
		for (int face = 0; face < 6; ++face)
		{
			shader.SetUniformVec4("uColor", FaceColor(face));
			m_mesh.Draw(face * vertsPerFace, vertsPerFace);
		}
	}

private:
	static std::vector<float> CreateVertices(float h)
	{
		return {
			-h, -h, h, h, -h, h, h, h, h, -h, -h, h, h, h, h, -h, h, h,
			h, -h, -h, -h, -h, -h, -h, h, -h, h, -h, -h, -h, h, -h, h, h, -h,
			h, -h, h, h, -h, -h, h, h, -h, h, -h, h, h, h, -h, h, h, h,
			-h, -h, -h, -h, -h, h, -h, h, h, -h, -h, -h, -h, h, h, -h, h, -h,
			-h, h, h, h, h, h, h, h, -h, -h, h, h, h, h, -h, -h, h, -h,
			-h, -h, -h, h, -h, -h, h, -h, h, -h, -h, -h, h, -h, h, -h, -h, h,
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