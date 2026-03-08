#pragma once
#include "../shared/Color.hpp"
#include "../shared/SceneObject.hpp"

class Circle : public SceneObject
{
public:
	Circle(Point center, Color color, float radius, int segments = 30)
		: SceneObject(center, 0, 1)
		, m_radius(radius)
		, m_color(color)
		, m_mesh(Mesh::Circle(radius, segments))

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
	float m_radius;
	Color m_color;
	Mesh m_mesh;
};