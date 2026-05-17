#pragma once

#include "QuadMesh.hpp"

#include <graphics/shaders/ShaderProgram.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <cmath>
#include <cstddef>
#include <string>

class ChinaFlag
{
public:
	static constexpr float Width = 30;
	static constexpr float Height = 20;

	ChinaFlag()
		: m_quad(glm::vec2(0, 0), glm::vec2(Width, Height))
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		BuildStars();
	}

	void Draw()
	{
		glm::mat4 proj = glm::ortho<float>(0, Width, Height, 0);

		m_shader.Use();
		m_shader.SetUniformMat4("uMVP", proj);
		for (std::size_t i = 0; i < m_stars.size(); ++i)
		{
			SetStarUniform(i, m_stars[i]);
		}

		m_quad.Draw();
	}

private:
	struct Star
	{
		glm::vec2 center;
		float radius;
		float rotation;
	};

	void SetStarUniform(std::size_t index, const Star& star)
	{
		const std::string prefix = "uStars[" + std::to_string(index) + "]";
		m_shader.SetUniformVec2(prefix + ".center", star.center);
		m_shader.SetUniformFloat(prefix + ".radius", star.radius);
		m_shader.SetUniformFloat(prefix + ".rotation", star.rotation);
	}

	void BuildStars()
	{
		const glm::vec2 bigCenter(5, 5);
		m_stars[0] = Star{ bigCenter, 3, 0 };

		constexpr std::array<glm::vec2, 4> smallCenters = {
			glm::vec2(10, 2),
			glm::vec2(12, 4),
			glm::vec2(12, 7),
			glm::vec2(10, 9),
		};
		for (std::size_t i = 0; i < smallCenters.size(); ++i)
		{
			glm::vec2 directionToBigStar = bigCenter - smallCenters[i];
			float rotation = std::atan2(directionToBigStar.x, -directionToBigStar.y);
			m_stars[i + 1] = Star{ smallCenters[i], 1, rotation };
		}
	}

	ShaderProgram m_shader;
	QuadMesh m_quad;
	std::array<Star, 5> m_stars{};
};
