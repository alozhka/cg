#pragma once

#include <GLFW/glfw3.h>

class KeyboardReader
{
public:
	explicit KeyboardReader(GLFWwindow* window)
		: m_window(window)
	{
	}

	bool IsButtonPressed(int button) const
	{
		return IsWindowAvailable() && glfwGetKey(m_window, button) == GLFW_PRESS;
	}

private:
	bool IsWindowAvailable() const
	{
		return m_window != nullptr;
	}

	GLFWwindow* m_window;
};