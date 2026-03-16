#pragma once
#include "../../graphics/Color.hpp"
#include "../../graphics/Drawable.hpp"
#include "../../graphics/Mesh.hpp"
#include "../viewModel/SpaceshipViewModel.hpp"

class SpaceshipView
{
public:
	explicit SpaceshipView(const SpaceshipViewModel& viewModel)
		: m_viewModel(viewModel)
		, m_spaceshipMesh(CreateSpaceshipMesh())
		, m_flameMesh(CreateFlameMesh())
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& projection)
	{
		Vec2f pos = m_viewModel.GetPosition();
		Mat3 mvp = projection
			* Mat3::Translate(pos.x, pos.y)
			* Mat3::Rotate(m_viewModel.GetAngle());

		shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
		shader.SetUniformVec4("uColor", m_shipColor.ToFloatArray());

		m_spaceshipMesh.Draw();

		if (m_viewModel.IsThrusting())
		{
			shader.SetUniformVec4("uColor", m_flameColor.ToFloatArray());
			m_flameMesh.Draw();
		}
	}

private:
	Mesh CreateSpaceshipMesh() const
	{
		std::vector<float> vertices = m_viewModel.ListVertices();
		return Mesh{ vertices, GL_LINE_LOOP };
	}

	Mesh CreateFlameMesh() const
	{
		std::vector<float> vertices = m_viewModel.ListFlameVertices();
		return Mesh{ vertices, GL_TRIANGLE_FAN };
	}

	SpaceshipViewModel m_viewModel;
	Mesh m_spaceshipMesh;
	Mesh m_flameMesh;

	Color m_shipColor{ 1, 1, 1, 1 };
	Color m_flameColor{ 1, 0.5, 0, 1 };
};