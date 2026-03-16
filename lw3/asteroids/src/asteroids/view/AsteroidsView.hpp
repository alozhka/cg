#pragma once
#include "../../graphics/Color.hpp"
#include "../../graphics/Mat3.hpp"
#include "../../graphics/Mesh.hpp"
#include "../../graphics/ShaderProgram.hpp"

class AsteroidsView
{
public:
	explicit AsteroidsView(const AsteroidsViewModel& asteroidsView)
		: m_viewModel(asteroidsView)
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& projection)
	{
		shader.SetUniformVec4("uColor", color.ToFloatArray());

		for (AsteroidDto& a : m_viewModel.ListAsteroids())
		{
			Mat3 mvp = projection
				* Mat3::Translate(a.position.x, a.position.y)
				* Mat3::Rotate(a.angle);
			shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
			Mesh mesh = Mesh{ a.vertices, GL_LINE_LOOP };
			mesh.Draw();
		}
	}

private:
	AsteroidsViewModel m_viewModel;
	Color color{ 0.8, 0.8, 0.8, 1 };
};