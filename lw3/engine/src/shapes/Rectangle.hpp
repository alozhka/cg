#pragma once
#include "../render/Mesh.hpp"
#include "../shared/Color.hpp"
#include "../shared/SceneObject.hpp"

class Rectangle : public SceneObject
{
public:
	Rectangle(Point p, double width, double height, const Color& color, double rotation = 0)
		: SceneObject(p, rotation, 1)
		, m_width(width)
		, m_height(height)
		, m_color(color)
		, m_mesh(Mesh::Rectangle(width, height))
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& parentTransform) override
	{
		Mat3 world = parentTransform * GetTransformMatrix();

		shader.SetUniformMat3("uViewProjection", world.ToFloatArray());
		shader.SetUniformVec4("uColor", m_color.ToFloatArray());

		m_mesh.Draw();
	}

private:
	double m_width;
	double m_height;
	Color m_color;
	Mesh m_mesh;
};