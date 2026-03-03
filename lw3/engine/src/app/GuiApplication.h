#pragma once
#define GLFW_INCLUDE_NONE
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
	struct Size
	{
		int width = 0, height = 0;
	};

	void ApplyProjectionMatrix();
	Point NormalizeCoords(double x, double y) const;
	void SetupInitialViewport();
	void SetupGlad();

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void FrameBufferSizeCallback(GLFWwindow*, int width, int height);

	Size GetWindowSize() const;
	static GuiApplication* GetInstance(GLFWwindow* window);

	GLFWwindow* m_window;
};
