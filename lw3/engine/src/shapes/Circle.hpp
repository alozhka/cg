#pragma once
#include "../shared/Color.hpp"
#include "../shared/SceneObject.hpp"
#include "../render/ShaderProgram.hpp"
#include <glad/glad.h>
#include <numbers>
#include <cmath>
#include <vector>

class Circle : public SceneObject
{
public:
	Circle(Point center, Color color, float radius, int segments = 30);
	~Circle();

	void Draw(ShaderProgram& shader, const Mat3& parentModel) override;

private:
	float m_radius;
	int m_segments;
	Color m_color;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
};
