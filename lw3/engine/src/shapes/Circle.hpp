#pragma once
#include "../graphics/Color.hpp"
#include "../graphics/Drawable.hpp"
#include "../graphics/Mesh.hpp"

class Circle : public Drawable
{
public:
	Circle(Vec2f center, Color color, float radius, int segments = 30)
		: Drawable(center, 0, 1)
		, m_radius(radius)
		, m_color(color)
		, m_mesh(CreateMesh(radius, segments))

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
	static Mesh CreateMesh(float radius, int segments = 30)
	{
		std::vector<float> vertices;
		vertices.reserve(segments * 2);
		vertices.push_back(0);
		vertices.push_back(0);

		for (int i = 0; i <= segments; i++)
		{
			float theta = 2 * std::numbers::pi_v<float> * i / segments;

			vertices.push_back(radius * std::cos(theta));
			vertices.push_back(radius * std::sin(theta));
		}

		return Mesh{ vertices, GL_TRIANGLE_FAN };
	}

	float m_radius;
	Color m_color;
	Mesh m_mesh;
};