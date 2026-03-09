#pragma once

#include "../graphics/Color.hpp"
#include "../graphics/Mat3.hpp"
#include "../graphics/Mesh.hpp"
#include "../graphics/ShaderProgram.hpp"
#include "../model/Asteroid.hpp"

#include <memory>
#include <unordered_map>

class AsteroidView
{
public:
	void Render(ShaderProgram& shader, const Mat3& projection, const std::vector<Asteroid>& asteroids)
	{
		for (size_t i = 0; i < asteroids.size(); i++)
		{
			const auto& ast = asteroids[i];

			EnsureMesh(i, ast);

			Mat3 model = Mat3::Translate(ast.position.x, ast.position.y)
				* Mat3::Rotate(ast.angle);
			Mat3 mvp = projection * model;

			shader.SetUniformMat3("uViewProjection", mvp.ToFloatArray());
			shader.SetUniformVec4("uColor", m_color.ToFloatArray());
			m_meshes[i]->Draw();
		}

		if (m_meshes.size() > asteroids.size() + 10)
		{
			while (m_meshes.size() > asteroids.size())
			{
				m_meshes.erase(std::prev(m_meshes.end()));
			}
		}
	}

	void ClearMeshes()
	{
		m_meshes.clear();
	}

private:
	void EnsureMesh(size_t idx, const Asteroid& ast)
	{
		if (m_meshes.count(idx) && m_vertexCounts[idx] == ast.vertices.size())
		{
			return;
		}

		std::vector<float> data;
		data.reserve(ast.vertices.size() * 2);
		for (const auto& v : ast.vertices)
		{
			data.push_back(v.x);
			data.push_back(v.y);
		}

		m_meshes[idx] = std::make_unique<Mesh>(data, GL_LINE_LOOP);
		m_vertexCounts[idx] = ast.vertices.size();
	}

	std::unordered_map<size_t, std::unique_ptr<Mesh>> m_meshes;
	std::unordered_map<size_t, size_t> m_vertexCounts;
	Color m_color{ 0.8f, 0.8f, 0.8f, 1.0f };
};
