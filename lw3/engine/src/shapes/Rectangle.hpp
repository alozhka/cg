#pragma once
#include "../graphics/Color.hpp"
#include "../graphics/Mesh.hpp"
#include "../graphics/SceneObject.hpp"

class Rectangle : public SceneObject
{
public:
	Rectangle(Vec2f p, float width, float height, const Color& color, float rotation = 0)
		: SceneObject(p, rotation, 1)
		, m_width(width)
		, m_height(height)
		, m_color(color)
		, m_mesh(CreateMesh(width, height))
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
	static Mesh CreateMesh(float width, float height)
	{
		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;
		return Mesh({ -halfWidth, -halfHeight,
						halfWidth, -halfHeight,
						halfWidth, halfHeight,
						-halfWidth, -halfHeight,
						halfWidth, halfHeight,
						-halfWidth, halfHeight },
			GL_TRIANGLES);
	}
	float m_width;
	float m_height;
	Color m_color;
	Mesh m_mesh;
};