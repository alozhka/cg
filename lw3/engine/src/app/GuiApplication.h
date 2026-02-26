#pragma once
#include "../shared/Point.hpp"
#include "GLFW/glfw3.h"

#include <string>

class GuiApplication
{
public:
	void MainLoop();

	GuiApplication(const GuiApplication&) = delete;
	GuiApplication& operator=(const GuiApplication&) = delete;

protected:
	GuiApplication(int width, int height, const std::string& title);
	virtual ~GuiApplication();

	virtual void OnDraw() = 0;
	virtual void OnMouseButton(int button, int action, Point p);
	virtual void OnMouseMove(Point p);

private:
	void ApplyProjectionMatrix();
	Point NormalizeCoords(double x, double y) const;
	void SetupInitialViewport();

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void FrameBufferSizeCallback(GLFWwindow*, int width, int height);

	static GuiApplication* GetInstance(GLFWwindow* window);

	GLFWwindow* m_window;
	int m_worldWidth = 0, m_worldHeight = 0;
	double m_currentAspectRatio = 0;
};
