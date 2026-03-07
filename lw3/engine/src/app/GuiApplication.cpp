#include "../app/GuiApplication.h"
#include <algorithm>
#include <glad/glad.h>
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
	SetupGlad();
	glfwSetFramebufferSizeCallback(m_window, &FrameBufferSizeCallback);
	glfwSetWindowUserPointer(m_window, this);
	SetupInitialViewport();

	glfwSetMouseButtonCallback(m_window, &GuiApplication::MouseButtonCallback);
	glfwSetCursorPosCallback(m_window, &GuiApplication::CursorPosCallback);
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
	if (GuiApplication* app = GetInstance(window))
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		app->OnMouseButton(button, action, app->NormalizeCoords(x, y));
	}
}

void GuiApplication::CursorPosCallback(GLFWwindow* window, double x, double y)
{
	if (GuiApplication* app = GetInstance(window))
	{
		app->OnMouseMove(app->NormalizeCoords(x, y));
	}
}

void GuiApplication::FrameBufferSizeCallback(GLFWwindow*, int width, int height)
{
	glViewport(0, 0, width, height);
}

GuiApplication::Size GuiApplication::GetWindowSize() const
{
	int w, h;
	glfwGetWindowSize(m_window, &w, &h);
	return { w, h };
}

GuiApplication* GuiApplication::GetInstance(GLFWwindow* window)
{
	return static_cast<GuiApplication*>(glfwGetWindowUserPointer(window));
}

Point GuiApplication::NormalizeCoords(double x, double y) const
{
	Size size = GetWindowSize();

	double normalizedX = (x / static_cast<double>(size.width)) * 2 - 1;
	double normalizedY = 1 - (y / static_cast<double>(size.height)) * 2;

	normalizedX = std::clamp(normalizedX, -1.0, 1.0);
	normalizedY = std::clamp(normalizedY, -1.0, 1.0);

	return Point{ normalizedX, normalizedY };
}

void GuiApplication::SetupInitialViewport()
{
	int w, h;
	glfwGetFramebufferSize(m_window, &w, &h);
	glViewport(0, 0, w, h);
}

void GuiApplication::SetupGlad()
{
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}
}

void GuiApplication::MainLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		UpdateProjectionMatrix();
		OnDraw();
		glfwSwapBuffers(m_window);
	}
}

void GuiApplication::UpdateProjectionMatrix()
{
	Size size = GetWindowSize();
	double viewWidth = size.width;
	double viewHeight = size.height;
	double currentAspectRatio = viewWidth / viewHeight;

	if (currentAspectRatio > 1)
	{
		viewWidth = viewHeight * currentAspectRatio;
	}
	else
	{
		viewHeight = viewWidth / currentAspectRatio;
	}

	double halfWidth = 0.5 * viewWidth;
	double halfHeight = 0.5 * viewHeight;

	m_projection = Mat3::Ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);
}