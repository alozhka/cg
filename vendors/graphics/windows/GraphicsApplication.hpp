#pragma once
#include "../Size.hpp"
#include "KeyboardReader.hpp"

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
			glm::mat4 perspective = GetPerspectiveMatrix();
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

	virtual void OnDraw(const glm::mat4& perspective) = 0;

	virtual void OnMouseButton(int button, int action, glm::vec2 p)
	{
	}

	virtual void OnMouseMove(glm::vec2 p)
	{
	}

	virtual void OnRawMouseMove(double x, double y)
	{
	}

	void SetWindowTitle(const std::string& title)
	{
		if (m_window)
		{
			glfwSetWindowTitle(m_window, title.c_str());
		}
	}

	void CaptureMouseInput()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	KeyboardReader CreateKeyboardReader() const
	{
		return KeyboardReader(m_window);
	}

private:
	glm::mat4 GetPerspectiveMatrix() const
	{
		Size size = GetWindowSize();
		float currentAspectRatio = size.width / size.height;

		return glm::perspective(m_fieldOfView, currentAspectRatio, m_zNear, m_zFar);
	}

	glm::vec2 NormalizeCoords(double x, double y) const
	{
		Size size = GetWindowSize();

		float normalizedX = (static_cast<float>(x) / static_cast<float>(size.width)) * 2 - 1;
		float normalizedY = 1 - (static_cast<float>(y) / static_cast<float>(size.height)) * 2;

		normalizedX = std::clamp(normalizedX, -1.0f, 1.0f);
		normalizedY = std::clamp(normalizedY, -1.0f, 1.0f);

		return glm::vec2{ normalizedX, normalizedY };
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
			app->OnRawMouseMove(x, y);
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

protected:
	float m_zNear = 0.1;
	float m_zFar = 100;
	float m_fieldOfView = 55;

private:

	GLFWwindow* m_window;
};
