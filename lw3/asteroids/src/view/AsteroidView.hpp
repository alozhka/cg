#pragma once

#include "../graphics/Color.hpp"
#include "../graphics/Mat3.hpp"
#include "../graphics/Mesh.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "../viewmodel/ViewData.hpp"

#include <memory>
#include <vector>

class AsteroidView
{
public:
	void Render(ShaderProgram& shader, const Mat3& projection, const AsteroidViewModel& ast)
	{
		EnsureMesh(ast.vertices);

		Mat3 model = Mat3::Translate(ast.position.x, ast.position.y)
			* Mat3::Rotate(ast.angle);
		Mat3 mvp = projection * model;

		shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
		shader.SetUniformVec4("uColor", m_color.ToFloatArray());
		m_mesh->Draw();
	}

	void ClearMesh()
	{
		m_mesh.reset();
		m_vertexCount = 0;
	}

private:
	void EnsureMesh(const std::vector<Vec2f>& vertices)
	{
		if (m_mesh && m_vertexCount == vertices.size()) return;

		std::vector<float> data;
		data.reserve(vertices.size() * 2);
		for (const auto& v : vertices)
		{
			data.push_back(v.x);
			data.push_back(v.y);
		}
		m_mesh = std::make_unique<Mesh>(data, GL_LINE_LOOP);
		m_vertexCount = vertices.size();
	}

	std::unique_ptr<Mesh> m_mesh;
	size_t m_vertexCount = 0;
	Color m_color{ 0.8f, 0.8f, 0.8f, 1.0f };
};
