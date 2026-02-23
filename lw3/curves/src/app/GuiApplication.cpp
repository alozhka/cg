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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, &FrameBufferSizeCallback);

	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

GuiApplication::~GuiApplication()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}
	glfwTerminate();
}

void GuiApplication::MainLoop(const RenderCallback& onDraw)
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		onDraw();
		glfwSwapBuffers(m_window);
	}
}