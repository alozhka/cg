#pragma once
#include "../../graphics/Color.hpp"
#include "../../graphics/Mat3.hpp"
#include "../../graphics/Mesh.hpp"
#include "../../graphics/ShaderProgram.hpp"
#include "../model/Debris.hpp"
#include "../viewModel/DebrisViewModel.hpp"

#include <vector>

class DebrisView
{
public:
	explicit DebrisView(const DebrisViewModel& viewModel)
		: m_viewModel(viewModel)
		, m_mesh(CreateMesh())
	{
	}

	void Draw(ShaderProgram& shader, const Mat3& projection)
	{
		shader.SetUniformVec4("uColor", m_color.ToFloatArray());

		for (const DebrisDto& d : m_viewModel.ListDebris())
		{
			Mat3 mvp = projection
				* Mat3::Translate(d.position.x, d.position.y)
				* Mat3::Rotate(d.angle);
			shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
			m_mesh.Draw();
		}
	}

private:
	static Mesh CreateMesh()
	{
		std::vector<Vec2f> vertices = Debris::ListVertices();
		std::vector<float> newVertices;
		newVertices.reserve(vertices.size() * 2);
		for (const Vec2f& v : vertices)
		{
			newVertices.push_back(v.x);
			newVertices.push_back(v.y);
		}
		return Mesh{ newVertices, GL_LINES };
	}

	DebrisViewModel m_viewModel;
	Mesh m_mesh;
	Color m_color{ 0.9f, 0.9f, 0.9f, 1 };
};
