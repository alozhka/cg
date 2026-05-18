#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>

class SurfaceMesh
{
public:
	SurfaceMesh(int uCount, int vCount)
	{
		const auto uv = BuildUVGrid(uCount, vCount);
		const auto indices = BuildTriangleIndices(uCount, vCount);
		DefineAttributes(uv, indices);
	}

	~SurfaceMesh()
	{
		glDeleteBuffers(1, &m_ebo);
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	SurfaceMesh(const SurfaceMesh&) = delete;
	SurfaceMesh& operator=(const SurfaceMesh&) = delete;

	void Draw() const
	{
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

private:
	static std::vector<glm::vec2> BuildUVGrid(int uCount, int vCount)
	{
		std::vector<glm::vec2> uv;
		uv.reserve(uCount * vCount);

		for (int vIdx = 0; vIdx < vCount + 1; ++vIdx)
		{
			const float v = static_cast<float>(vIdx) / vCount;
			for (int uIdx = 0; uIdx < uCount + 1; ++uIdx)
			{
				const float u = static_cast<float>(uIdx) / uCount;
				uv.emplace_back(u, v);
			}
		}
		return uv;
	}

	static std::vector<GLuint> BuildTriangleIndices(int uCount, int vCount)
	{
		const int uVertsCount = uCount + 1;

		std::vector<GLuint> indices;
		indices.reserve(6 * uCount * vCount);

		for (int v = 0; v < vCount; ++v)
		{
			for (int u = 0; u < uCount; ++u)
			{
				const GLuint i00 = FlatIndex(u,     v,     uVertsCount);
				const GLuint i10 = FlatIndex(u + 1, v,     uVertsCount);
				const GLuint i01 = FlatIndex(u,     v + 1, uVertsCount);
				const GLuint i11 = FlatIndex(u + 1, v + 1, uVertsCount);

				indices.push_back(i00);
				indices.push_back(i10);
				indices.push_back(i11);

				indices.push_back(i00);
				indices.push_back(i11);
				indices.push_back(i01);
			}
		}
		return indices;
	}

	static GLuint FlatIndex(int u, int v, int uVertsCount)
	{
		return static_cast<GLuint>(v * uVertsCount + u);
	}

	void DefineAttributes(const std::vector<glm::vec2>& uv, const std::vector<GLuint>& indices)
	{
		m_indexCount = static_cast<GLsizei>(indices.size());

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(uv.size() * sizeof(glm::vec2)),
			uv.data(),
			GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)),
			indices.data(),
			GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	GLsizei m_indexCount = 0;
};
