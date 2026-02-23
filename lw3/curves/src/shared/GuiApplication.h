#pragma once
#include "GLFW/glfw3.h"
#include "Point2D.hpp"

#include <string>

class GuiApplication
{
public:
	void MainLoop();

protected:
	GuiApplication(int width, int height, const std::string& title);
	virtual ~GuiApplication();

	virtual void OnDraw() = 0;
	virtual void OnMouseButton(int button, int action, Point2D p);
	virtual void OnMouseMove(Point2D p);

private:
	Point2D NormalizeCoords(double x, double y) const;

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void FrameBufferSizeCallback(GLFWwindow*, int width, int height);

	GLFWwindow* m_window;
};
