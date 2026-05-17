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
		const auto indices = BuildWireframeIndices(uCount, vCount);
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
		glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

private:
	static std::vector<glm::vec2> BuildUVGrid(int uCount, int vCount)
	{
		const int uVertsCount = uCount + 1;
		const int vVertsCount = vCount + 1;

		std::vector<glm::vec2> uv;
		uv.reserve(uVertsCount * vVertsCount);

		for (int vIdx = 0; vIdx < vVertsCount; ++vIdx)
		{
			const float v = vIdx / vCount;
			for (int uIdx = 0; uIdx < uVertsCount; ++uIdx)
			{
				const float u = uIdx / uCount;
				uv.emplace_back(u, v);
			}
		}
		return uv;
	}

	static std::vector<GLuint> BuildWireframeIndices(int uSteps, int vSteps)
	{
		const int uVerts = uSteps + 1;
		const int vVerts = vSteps + 1;

		// row-major: соседние по u лежат подряд
		auto flatIndex = [uVerts](int u, int v) {
			return static_cast<GLuint>(v * uVerts + u);
		};

		std::vector<GLuint> indices;
		indices.reserve(2 * (static_cast<size_t>(uSteps) * vVerts + static_cast<size_t>(vSteps) * uVerts));

		// Рёбра вдоль u: uSteps отрезков на каждой v-линии
		for (int v = 0; v < vVerts; ++v)
		{
			for (int u = 0; u < uSteps; ++u)
			{
				indices.push_back(flatIndex(u, v));
				indices.push_back(flatIndex(u + 1, v));
			}
		}

		// Рёбра вдоль v: vSteps отрезков на каждом u-меридиане
		for (int u = 0; u < uVerts; ++u)
		{
			for (int v = 0; v < vSteps; ++v)
			{
				indices.push_back(flatIndex(u, v));
				indices.push_back(flatIndex(u, v + 1));
			}
		}
		return indices;
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
