#pragma once

#include "CurveMesh.hpp"

#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/windows/GraphicsApplication.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class CannabolaApp final : public GraphicsApplication
{
public:
	CannabolaApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
		, m_curve(BuildVertices())
	{
		m_shader.LoadFromFile("assets/vertex.glsl", "assets/fragment.glsl");

		glEnable(GL_LINE_SMOOTH);
	}

protected:
	void OnDraw(const glm::mat4& perspective) override
	{
		glClearColor(0.08, 0.09, 0.11, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		constexpr glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(0, -1, -4));
		const glm::mat4 mvp = perspective * view;

		m_shader.Use();
		m_shader.SetUniformMat4("uMVP", mvp);
		m_shader.SetUniformVec3("uColor", glm::vec3(0.3, 1, 0.45));

		m_curve.Draw(GL_LINE_LOOP);
	}

private:
	static std::vector<float> BuildVertices()
	{
		constexpr int approxCount = 1000;
		constexpr float end = glm::two_pi<float>();
		constexpr float step = glm::pi<float>() / approxCount;

		std::vector<float> v;
		v.reserve(approxCount);
		for (float x = 0; x <= end; x += step)
		{
			v.push_back(x);
		}
		return v;
	}

	ShaderProgram m_shader;
	CurveMesh m_curve;
};
