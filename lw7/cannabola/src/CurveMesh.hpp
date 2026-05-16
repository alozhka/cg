#pragma once

#include <glad/glad.h>

#include <vector>

class CurveMesh
{
public:
	explicit CurveMesh(const std::vector<float>& xs)
		: m_count(static_cast<GLsizei>(xs.size()))
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(xs.size() * sizeof(float)),
			xs.data(),
			GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

		glBindVertexArray(0);
	}

	~CurveMesh()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	CurveMesh(const CurveMesh&) = delete;
	CurveMesh& operator=(const CurveMesh&) = delete;

	void Draw(GLenum mode) const
	{
		glBindVertexArray(m_vao);
		glDrawArrays(mode, 0, m_count);
		glBindVertexArray(0);
	}

private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLsizei m_count = 0;
};