#pragma once
#include "Vec2f.h"

#include <glad/glad.h>
#include <vector>

class Mesh
{
public:
	Mesh(const std::vector<Vec2f>& vertices, GLenum drawMode)
		: m_drawMode(drawMode)
		, m_vertexCount(vertices.size())
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		GLsizeiptr size = vertices.size() * sizeof(double);
		glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(Vec2f), nullptr);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void Draw()
	{
		glBindVertexArray(m_vao);
		glDrawArrays(m_drawMode, 0, m_vertexCount);
		glBindVertexArray(0);
	}

private:
	GLenum m_drawMode;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLsizei m_vertexCount = 0;
};