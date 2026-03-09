#pragma once

#include <cmath>
#include <glad/glad.h>
#include <numbers>
#include <vector>

class Mesh
{
public:
	Mesh(const std::vector<float>& vertices, GLenum drawMode)
		: m_drawMode(drawMode)
		, m_vertexCount(vertices.size() / 2)
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		GLsizeiptr size = vertices.size() * sizeof(float);
		glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
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

private:
	GLenum m_drawMode;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLsizei m_vertexCount = 0;
};