#pragma once

#include "QuadMesh.hpp"

#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <numbers>

class ChinaFlagApp final : public GraphicsApplication
{
public:
	static constexpr float kFlagWidth = 30.0f;
	static constexpr float kFlagHeight = 20.0f;

	ChinaFlagApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_quad(glm::vec2(0.0f, 0.0f), glm::vec2(kFlagWidth, kFlagHeight))
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");
		BuildStars();
	}

protected:
	void OnDraw(const glm::mat4&) override
	{
		ApplyLetterboxViewport();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// y: flag top = 0, bottom = kFlagHeight; ортогональ переворачивает Y.
		const glm::mat4 proj = glm::ortho(0.0f, kFlagWidth, kFlagHeight, 0.0f);

		m_shader.Use();
		m_shader.SetUniformMat4("uMVP", proj);
		m_shader.SetUniformVec3("uBackground", glm::vec3(0xEE, 0x1C, 0x25) / 255.0f);
		m_shader.SetUniformVec3("uStarColor", glm::vec3(1.0f, 1.0f, 0.0f));
		for (size_t i = 0; i < m_stars.size(); ++i)
		{
			m_shader.SetUniformVec4("uStars[" + std::to_string(i) + "]", m_stars[i]);
		}

		m_quad.Draw();
	}

private:
	void BuildStars()
	{
		const glm::vec2 bigCenter(5.0f, 5.0f);
		// Большая звезда: одна вершина смотрит вверх.
		// В локальном фрейме SDF "tip" направлен вверх (+Y). После переворота Y
		// ортопроекцией это требует поворота на π (или на 0 — проверится).
		m_stars[0] = glm::vec4(bigCenter, 3.0f, 0.0f);

		constexpr std::array<glm::vec2, 4> smallCenters = {
			glm::vec2(10.0f, 2.0f),
			glm::vec2(12.0f, 4.0f),
			glm::vec2(12.0f, 7.0f),
			glm::vec2(10.0f, 9.0f),
		};
		for (size_t i = 0; i < smallCenters.size(); ++i)
		{
			glm::vec2 d = bigCenter - smallCenters[i];
			// SDF: tip направлен в +Y локального фрейма => нужен угол поворота,
			// при котором +Y локального совпадает с направлением d (мир, y вниз).
			// angle = atan2(d.x, -d.y) — поворот против часовой в мире с y-вниз.
			float angle = std::atan2(d.x, -d.y);
			m_stars[i + 1] = glm::vec4(smallCenters[i], 1.0f, angle);
		}
	}

	void ApplyLetterboxViewport() const
	{
		int fbw = 0;
		int fbh = 0;
		glfwGetFramebufferSize(glfwGetCurrentContext(), &fbw, &fbh);

		const float targetAspect = kFlagWidth / kFlagHeight;
		const float windowAspect = static_cast<float>(fbw) / static_cast<float>(fbh);

		int vpW = fbw;
		int vpH = fbh;
		if (windowAspect > targetAspect)
		{
			vpW = static_cast<int>(fbh * targetAspect);
		}
		else
		{
			vpH = static_cast<int>(fbw / targetAspect);
		}
		int vpX = (fbw - vpW) / 2;
		int vpY = (fbh - vpH) / 2;

		glViewport(vpX, vpY, vpW, vpH);
	}

	ShaderProgram m_shader;
	QuadMesh m_quad;
	// xy = центр, z = внешний радиус, w = угол (рад)
	std::array<glm::vec4, 5> m_stars{};
};