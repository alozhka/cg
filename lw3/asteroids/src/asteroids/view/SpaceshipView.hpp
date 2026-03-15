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
		, m_mesh(CreateMesh())
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& projection)
	{
		Vec2f pos = m_viewModel.GetPosition();
		Mat3 mvp = Mat3::Translate(pos.x, pos.y)
			* Mat3::Rotate(m_viewModel.GetAngle())
			* projection;

		shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
		shader.SetUniformVec4("uColor", m_shipColor.ToFloatArray());

		m_mesh.Draw();
	}

private:
	Mesh CreateMesh() const
	{
		std::vector<float> vertices = m_viewModel.ListVertices();
		return Mesh{ vertices, GL_LINE_LOOP };
	}

	SpaceshipViewModel m_viewModel;
	Mesh m_mesh;

	Color m_shipColor{ 1, 1, 1, 1 };
	Color m_flameColor{ 1, 0.5, 0, 1 };
};