#pragma once
#define GL_SILENCE_DEPRECATION
#include "HangmanViewModel.hpp"
#include "view/GameView.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class WindowManager
{
public:
	WindowManager(int width, int height, const std::string& title)
		: m_width(width)
		, m_height(height)
		, m_title(title)
		, m_viewModel("HANGMAN")
		, m_view(m_viewModel, width, height)
	{
	}

	bool init()
	{
		if (!glfwInit())
		{
			std::cerr << "Failed to initialize GLFW" << std::endl;
			return false;
		}

		m_window = glfwCreateWindow(
			m_width,
			m_height,
			m_title.c_str(),
			nullptr,
			nullptr);
		if (!m_window)
		{
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);

		glfwSetMouseButtonCallback(m_window, MouseButtonWindowProc);
		glfwSetFramebufferSizeCallback(m_window, ResizeWindowProc);

		return true;
	}

	void run()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			m_view.draw();

			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		glfwTerminate();
	}

private:
	static void MouseButtonWindowProc(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			HandleLeftButtonPress(window);
		}
	}

	static void ResizeWindowProc(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		auto* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
		if (wm)
		{
			wm->m_view.setWindowSize(width, height);
		}
	}

	static void HandleLeftButtonPress(GLFWwindow* window)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		auto* wm = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
		if (wm)
		{
			wm->m_view.handleClick(xpos, ypos);
		}
	}

	GLFWwindow* m_window = nullptr;
	int m_width;
	int m_height;
	std::string m_title;

	HangmanViewModel m_viewModel;
	GameView m_view;
};
