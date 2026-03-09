#pragma once

#include "AsteroidView.hpp"
#include "../viewmodel/ViewData.hpp"

#include <vector>

class AsteroidsView
{
public:
	void Render(ShaderProgram& shader, const Mat3& projection, const std::vector<AsteroidViewModel>& asteroids)
	{
		if (m_views.size() < asteroids.size())
		{
			m_views.resize(asteroids.size());
		}

		for (size_t i = 0; i < asteroids.size(); i++)
		{
			m_views[i].Render(shader, projection, asteroids[i]);
		}
	}

	void ClearMeshes()
	{
		m_views.clear();
	}

private:
	std::vector<AsteroidView> m_views;
};
