#include "../app/GuiApplication.h"

#include <algorithm>
#include <stdexcept>

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
	glfwSetWindowUserPointer(m_window, this);

	glfwSetMouseButtonCallback(m_window, &GuiApplication::MouseButtonCallback);
	glfwSetCursorPosCallback(m_window, &GuiApplication::CursorPosCallback);

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

void GuiApplication::OnMouseButton(int button, int action, Point p)
{
}

void GuiApplication::OnMouseMove(Point p)
{
}

void GuiApplication::MouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	auto* app = static_cast<GuiApplication*>(glfwGetWindowUserPointer(window));
	app->OnMouseButton(button, action, app->NormalizeCoords(x, y));
}

void GuiApplication::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	auto* app = static_cast<GuiApplication*>(glfwGetWindowUserPointer(window));
	app->OnMouseMove(app->NormalizeCoords(x, y));
}

void GuiApplication::FrameBufferSizeCallback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}

Point GuiApplication::NormalizeCoords(double x, double y) const
{
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);

	double normalizedX = (x / static_cast<double>(w)) * 2 - 1;
	double normalizedY = 1 - (y / static_cast<double>(h)) * 2;

	normalizedX = std::clamp(normalizedX, -1.0, 1.0);
	normalizedY = std::clamp(normalizedY, -1.0, 1.0);

	return Point{ normalizedX, normalizedY };
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