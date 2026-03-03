#pragma once
#include "../shared/Color.hpp"
#include "../shared/SceneObject.hpp"
#include "../render/ShaderProgram.hpp"
#include <glad/glad.h>

class Rectangle : public SceneObject
{
public:
	Rectangle(Point p, double width, double height, const Color& color, double rotation = 0);
	~Rectangle();

	void Draw(ShaderProgram& shader, const Mat3& parentModel) override;

private:
	double m_width;
	double m_height;
	Color m_color;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
};
