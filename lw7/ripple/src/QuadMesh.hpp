#pragma once

#include <glad/glad.h>

class QuadMesh
{
public:
	QuadMesh()
	{
		constexpr float data[] = {
			-1, -1,
			 1, -1,
			-1,  1,
			 1,  1,
		};

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glBindVertexArray(0);
	}

	~QuadMesh()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}

	QuadMesh(const QuadMesh&) = delete;
	QuadMesh& operator=(const QuadMesh&) = delete;

	void Draw() const
	{
		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
};
