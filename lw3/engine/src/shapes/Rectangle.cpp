#include "Rectangle.hpp"
#include "../shared/Mat3.hpp"

Rectangle::Rectangle(Point p, double width, double height, const Color& color, double rotation)
	: SceneObject(p, rotation, 1)
	, m_width(width)
	, m_height(height)
	, m_color(color)
{
	double cx = width / 2;
	double cy = height / 2;
	float vertices[] = {
		static_cast<float>(-cx), static_cast<float>(-cy),
		static_cast<float>(cx),  static_cast<float>(-cy),
		static_cast<float>(cx),  static_cast<float>(cy),
		static_cast<float>(-cx), static_cast<float>(cy)
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

Rectangle::~Rectangle()
{
	if (m_vao)
		glDeleteVertexArrays(1, &m_vao);
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
}

void Rectangle::Draw(ShaderProgram& shader, const Mat3& parentModel)
{
	Mat3 mvp = parentModel * GetModelMatrix();
	shader.Use();
	shader.SetMat3("uMVP", mvp.data());
	shader.SetVec4("uColor", static_cast<float>(m_color.r), static_cast<float>(m_color.g),
		static_cast<float>(m_color.b), static_cast<float>(m_color.a));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}
