#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>

class QuadMesh
{
public:
	QuadMesh(glm::vec2 min, glm::vec2 max)
	{
		const std::array<float, 8> data = {
			min.x, min.y,
			max.x, min.y,
			min.x, max.y,
			max.x, max.y,
		};

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data.data(), GL_STATIC_DRAW);

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