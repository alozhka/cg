#pragma once
#include "../../graphics/Color.hpp"
#include "../../graphics/Mesh.hpp"

#include <vector>

class BulletsView
{
public:
	explicit BulletsView(const BulletsViewModel& viewModel)
		: m_viewModel(viewModel)
		, m_mesh(CreateMesh())
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& projection)
	{
		shader.SetUniformVec4("uColor", m_color.ToFloatArray());

		for (auto bulletPos : m_viewModel.ListBulletsPositions())
		{
			Mat3 mvp = projection * Mat3::Translate(bulletPos.x, bulletPos.y);
			shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
			m_mesh.Draw();
		}
	}

private:
	static Mesh CreateMesh()
	{
		std::vector<float> vertices = BulletsViewModel::ListBulletVertices();
		return Mesh{ vertices, GL_TRIANGLES };
	}

	BulletsViewModel m_viewModel;
	Mesh m_mesh;
	Color m_color = { 1, 1, 0.5, 1 };
};