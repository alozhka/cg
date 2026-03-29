#pragma once

#include "Vertex.hpp"

#include <glad/glad.h>
#include <vector>

class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertices, GLenum drawMode)
		: m_drawMode(drawMode)
		, m_vertexCount(static_cast<GLsizei>(vertices.size()))
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), reinterpret_cast<const float*>(vertices.data()), GL_STATIC_DRAW);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			reinterpret_cast<void*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept
		: m_drawMode(other.m_drawMode)
		, m_vao(other.m_vao)
		, m_vbo(other.m_vbo)
		, m_vertexCount(other.m_vertexCount)
	{
		other.m_vao = 0;
		other.m_vbo = 0;
	}

	void Draw()
	{
		glBindVertexArray(m_vao);
		glDrawArrays(m_drawMode, 0, m_vertexCount);
		glBindVertexArray(0);
	}

	void Draw(GLint first, GLsizei count)
	{
		glBindVertexArray(m_vao);
		glDrawArrays(m_drawMode, first, count);
		glBindVertexArray(0);
	}

	GLuint GetVAO() const { return m_vao; }

private:
	GLenum m_drawMode;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLsizei m_vertexCount = 0;
};