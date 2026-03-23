#pragma once
#define GLFW_INCLUDE_NONE
#include "../../Mat4.hpp"
#include "../../Vec2f.hpp"
#include "../../Size.hpp"
#include "../KeyboardReader.hpp"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glad/glad.h>
#include <stdexcept>
#include <string>

class GraphicsApplication
{
public:
	void MainLoop()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();
			Mat4 perspective = GetPerspectiveMatrix();
			OnDraw(perspective);
			glfwSwapBuffers(m_window);
		}
	}

	GraphicsApplication(const GraphicsApplication&) = delete;
	GraphicsApplication& operator=(const GraphicsApplication&) = delete;

protected:
	GraphicsApplication(int width, int height, const std::string& title)
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
		SetupGlad();
		glfwSetFramebufferSizeCallback(m_window, &FrameBufferSizeCallback);
		glfwSetWindowUserPointer(m_window, this);
		SetupInitialViewport();

		glfwSetMouseButtonCallback(m_window, &GraphicsApplication::MouseButtonCallback);
		glfwSetCursorPosCallback(m_window, &GraphicsApplication::CursorPosCallback);
	}

	virtual ~GraphicsApplication()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
		}
		glfwTerminate();
	}

	virtual void OnDraw(const Mat4& projection) = 0;

	virtual void OnMouseButton(int button, int action, Vec2f p)
	{
	}

	virtual void OnMouseMove(Vec2f p)
	{
	}

	void SetWindowTitle(const std::string& title)
	{
		if (m_window)
		{
			glfwSetWindowTitle(m_window, title.c_str());
		}
	}

	KeyboardReader CreateKeyboardReader() const
	{
		return KeyboardReader(m_window);
	}

private:
	Mat4 GetPerspectiveMatrix() const
	{
		Size size = GetWindowSize();
		float viewWidth = size.width;
		float viewHeight = size.height;
		float currentAspectRatio = viewWidth / viewHeight;

		if (currentAspectRatio > 1)
		{
			viewWidth = viewHeight * currentAspectRatio;
		}
		else
		{
			viewHeight = viewWidth / currentAspectRatio;
		}

		float halfWidth = 0.5 * viewWidth;
		float halfHeight = 0.5 * viewHeight;

		return Mat4::Perspective(-halfWidth, halfWidth, -halfHeight, halfHeight);
	}

	Vec2f NormalizeCoords(double x, double y) const
	{
		Size size = GetWindowSize();

		float normalizedX = (static_cast<float>(x) / static_cast<float>(size.width)) * 2 - 1;
		float normalizedY = 1 - (static_cast<float>(y) / static_cast<float>(size.height)) * 2;

		normalizedX = std::clamp(normalizedX, -1.0f, 1.0f);
		normalizedY = std::clamp(normalizedY, -1.0f, 1.0f);

		return Vec2f{ normalizedX, normalizedY };
	}

	void SetupInitialViewport()
	{
		int w, h;
		glfwGetFramebufferSize(m_window, &w, &h);
		glViewport(0, 0, w, h);
	}

	void SetupGlad()
	{
		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
			throw std::runtime_error("Failed to initialize GLAD");
		}
	}

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int)
	{
		if (GraphicsApplication* app = GetInstance(window))
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			app->OnMouseButton(button, action, app->NormalizeCoords(x, y));
		}
	}
	static void CursorPosCallback(GLFWwindow* window, double x, double y)
	{
		if (GraphicsApplication* app = GetInstance(window))
		{
			app->OnMouseMove(app->NormalizeCoords(x, y));
		}
	}

	static void FrameBufferSizeCallback(GLFWwindow*, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	Size GetWindowSize() const
	{
		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		return { w, h };
	}

	static GraphicsApplication* GetInstance(GLFWwindow* window)
	{
		return static_cast<GraphicsApplication*>(glfwGetWindowUserPointer(window));
	}

	static constexpr float ZNEAR = 0.5;
	static constexpr float ZFAR = 10;
	static constexpr float FIELD_OF_VIEW = 60;

	GLFWwindow* m_window;
};
