#include "../app/GuiApplication.h"
#include <glad/glad.h>

#include <algorithm>
#include <stdexcept>

GuiApplication::GuiApplication(int width, int height, const std::string& title)
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to initialize window");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glfwSetFramebufferSizeCallback(m_window, &FrameBufferSizeCallback);
	glfwSetWindowUserPointer(m_window, this);
	SetupInitialViewport();
	UpdateProjectionMatrix();

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

void GuiApplication::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (GuiApplication* app = GetInstance(window))
		app->UpdateProjectionMatrix();
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

void GuiApplication::MainLoop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		OnDraw();
		glfwSwapBuffers(m_window);
	}
}

void GuiApplication::UpdateProjectionMatrix()
{
	Size size = GetWindowSize();
	double viewWidth = static_cast<double>(size.width);
	double viewHeight = static_cast<double>(size.height);
	double currentAspectRatio = viewWidth / viewHeight;

	if (currentAspectRatio > 1)
		viewWidth = viewHeight * currentAspectRatio;
	else
		viewHeight = viewWidth / currentAspectRatio;

	double left = -viewWidth * 0.5;
	double right = viewWidth * 0.5;
	double bottom = -viewHeight * 0.5;
	double top = viewHeight * 0.5;

	// Orthographic 3x3: scale and translate to NDC-like range
	// (x,y) in [left,right] x [bottom,top] -> [-1,1] x [-1,1]
	float sx = 2.f / static_cast<float>(right - left);
	float sy = 2.f / static_cast<float>(top - bottom);
	float tx = -static_cast<float>(left + right) / static_cast<float>(right - left);
	float ty = -static_cast<float>(bottom + top) / static_cast<float>(top - bottom);

	m_projection = Mat3::Identity();
	m_projection.m[0] = sx;
	m_projection.m[4] = sy;
	m_projection.m[6] = tx;
	m_projection.m[7] = ty;
}