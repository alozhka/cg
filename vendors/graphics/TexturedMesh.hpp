#pragma once

#include "TexturedVertex.hpp"

#include <glad/glad.h>
#include <vector>

class TexturedMesh
{
public:
	TexturedMesh(const std::vector<TexturedVertex>& vertices)
		: m_vertexCount(static_cast<GLsizei>(vertices.size()))
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedVertex), reinterpret_cast<const float*>(vertices.data()), GL_STATIC_DRAW);

		DefineTexturedVertex();

		glBindVertexArray(0);
	}

	TexturedMesh(const std::vector<TexturedVertex>& vertices, const std::vector<GLuint>& indices)
		: m_vertexCount(static_cast<GLsizei>(vertices.size()))
		, m_indexCount(static_cast<GLsizei>(indices.size()))
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TexturedVertex), reinterpret_cast<const float*>(vertices.data()), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

		DefineTexturedVertex();

		glBindVertexArray(0);
	}

	~TexturedMesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		if (m_ebo)
		{
			glDeleteBuffers(1, &m_ebo);
		}
	}

	TexturedMesh(const TexturedMesh&) = delete;
	TexturedMesh& operator=(const TexturedMesh&) = delete;

	TexturedMesh(TexturedMesh&& other) noexcept
		: m_vao(other.m_vao)
		, m_vbo(other.m_vbo)
		, m_ebo(other.m_ebo)
		, m_vertexCount(other.m_vertexCount)
		, m_indexCount(other.m_indexCount)
	{
		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
	}

	void Draw(GLenum drawMode)
	{
		glBindVertexArray(m_vao);
		if (m_indexCount > 0)
		{
			glDrawElements(drawMode, m_indexCount, GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			glDrawArrays(drawMode, 0, m_vertexCount);
		}
		glBindVertexArray(0);
	}

	void Draw(GLenum drawMode, GLint first, GLsizei count)
	{
		glBindVertexArray(m_vao);
		glDrawArrays(drawMode, first, count);
		glBindVertexArray(0);
	}

	GLuint GetVAO() const { return m_vao; }

private:
	static void DefineTexturedVertex()
	{
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(TexturedVertex),
			reinterpret_cast<void*>(offsetof(TexturedVertex, position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(TexturedVertex),
			reinterpret_cast<void*>(offsetof(TexturedVertex, normal)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(TexturedVertex),
			reinterpret_cast<void*>(offsetof(TexturedVertex, texCoord)));
		glEnableVertexAttribArray(2);
	}
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	GLsizei m_vertexCount = 0;
	GLsizei m_indexCount = 0;
};