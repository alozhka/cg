#pragma once

#include "ChinaFlag.hpp"

#include <graphics/windows/GraphicsApplication.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

class ChinaFlagApp final : public GraphicsApplication
{
public:
	ChinaFlagApp(int width, int height, const std::string& title)
		: GraphicsApplication(width, height, title)
	{
	}

protected:
	void OnDraw(const glm::mat4&) override
	{
		ApplyLetterboxViewport();

		glClearColor(0.05, 0.05, 0.05, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_flag.Draw();
	}

private:
	static void ApplyLetterboxViewport()
	{
		int fbw = 0;
		int fbh = 0;
		glfwGetFramebufferSize(glfwGetCurrentContext(), &fbw, &fbh);

		constexpr float targetAspect = ChinaFlag::Width / ChinaFlag::Height;
		const float windowAspect = fbw / fbh;

		int vpW = fbw;
		int vpH = fbh;
		if (windowAspect > targetAspect)
		{
			vpW = fbh * targetAspect;
		}
		else
		{
			vpH = fbw / targetAspect;
		}
		int vpX = (fbw - vpW) / 2;
		int vpY = (fbh - vpH) / 2;

		glViewport(vpX, vpY, vpW, vpH);
	}

	ChinaFlag m_flag;
};
