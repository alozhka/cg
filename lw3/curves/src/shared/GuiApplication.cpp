#include "GuiApplication.h"

#include <stdexcept>

namespace
{
void FrameBufferSizeCallback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}
} // namespace

GuiApplication::GuiApplication(int width, int height, const std::string& title)
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize window");
	}

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, &FrameBufferSizeCallback);

	glViewport(0, 0, width, height);
	glClearColor(0.2, 0.2, 0.2, 1);
}

GuiApplication::~GuiApplication()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}
	glfwTerminate();
}

void GuiApplication::MainLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		OnDraw();
		glfwSwapBuffers(m_window);
	}
}