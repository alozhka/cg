#pragma once
#include "GLFW/glfw3.h"

#include <string>

class GuiApplication
{
public:
	void MainLoop();

protected:
	GuiApplication(int width, int height, const std::string& title);
	virtual ~GuiApplication();

	virtual void OnDraw() = 0;

private:
	GLFWwindow* m_window;
};
