#pragma once

#include <GLFW/glfw3.h>

#include <unordered_map>

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

	bool WasJustPressed(int button)
	{
		bool pressed = IsButtonPressed(button);
		bool justPressed = pressed && !m_pressed[button];
		m_pressed[button] = pressed;
		return justPressed;
	}

private:
	bool IsWindowAvailable() const
	{
		return m_window != nullptr;
	}

	GLFWwindow* m_window;
	std::unordered_map<int, bool> m_pressed;
};