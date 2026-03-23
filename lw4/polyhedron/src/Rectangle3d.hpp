#pragma once
#include <glad/glad.h>
#include <graphics/Mesh.hpp>
#include <vector>

class Rectangle3d
{
public:
	Rectangle3d(float width, float height)
		: m_mesh(CreateVertices(width, height), GL_TRIANGLES, 3)
	{
	}

	void Draw()
	{
		m_mesh.Draw();
	}

private:
	static std::vector<float> CreateVertices(float width, float height)
	{
		const float hw = width * 0.5f;
		const float hh = height * 0.5f;
		// 2 треугольника
		return {
			-hw, -hh, 0.0f,
			hw, -hh, 0.0f,
			hw, hh, 0.0f,
			-hw, -hh, 0.0f,
			hw, hh, 0.0f,
			-hw, hh, 0.0f
		};
	}
	Mesh m_mesh;
};