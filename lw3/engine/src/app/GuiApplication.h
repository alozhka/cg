#pragma once
#include "../shared/Point.hpp"
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
	virtual void OnMouseButton(int button, int action, Point p);
	virtual void OnMouseMove(Point p);

private:
	Point NormalizeCoords(double x, double y) const;

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void FrameBufferSizeCallback(GLFWwindow*, int width, int height);

	GLFWwindow* m_window;
};
