#include "Circle.hpp"
#include "../shared/Mat3.hpp"

Circle::Circle(Point center, Color color, float radius, int segments)
	: SceneObject(center, 0, 1)
	, m_radius(radius)
	, m_segments(segments)
	, m_color(color)
{
	std::vector<float> vertices;
	vertices.push_back(0.f);
	vertices.push_back(0.f);
	for (int i = 0; i <= m_segments; ++i)
	{
		double theta = 2 * std::numbers::pi * static_cast<double>(i) / static_cast<double>(m_segments);
		vertices.push_back(static_cast<float>(m_radius * std::cos(theta)));
		vertices.push_back(static_cast<float>(m_radius * std::sin(theta)));
	}

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

Circle::~Circle()
{
	if (m_vao)
		glDeleteVertexArrays(1, &m_vao);
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
}

void Circle::Draw(ShaderProgram& shader, const Mat3& parentModel)
{
	Mat3 mvp = parentModel * GetModelMatrix();
	shader.Use();
	shader.SetMat3("uMVP", mvp.data());
	shader.SetVec4("uColor", static_cast<float>(m_color.r), static_cast<float>(m_color.g),
		static_cast<float>(m_color.b), static_cast<float>(m_color.a));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_segments + 2);
	glBindVertexArray(0);
}
