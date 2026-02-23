#pragma once
#include "GLFW/glfw3.h"

#include <functional>
#include <string>

using RenderCallback = std::function<void()>;

class GuiApplication
{
public:
	GuiApplication(int width, int height, const std::string& title);
	~GuiApplication();

	GuiApplication(const GuiApplication&) = delete;
	GuiApplication& operator=(const GuiApplication&) = delete;

	void MainLoop(const RenderCallback& onDraw);

private:
	GLFWwindow* m_window;
};
