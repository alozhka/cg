#pragma once
#define GLFW_INCLUDE_NONE
#include "../graphics/Mat3.hpp"
#include "../graphics/Vec2f.hpp"
#include "GLFW/glfw3.h"
#include "KeyboardReader.hpp"

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

	virtual void OnDraw(const Mat3& projection) = 0;
	virtual void OnMouseButton(int button, int action, Vec2f p);
	virtual void OnMouseMove(Vec2f p);

	void SetWindowTitle(const std::string& title);
	KeyboardReader CreateKeyboardReader() const;

private:
	struct Size
	{
		int width = 0, height = 0;
	};

	Mat3 GetProjectionMatrix() const;
	Vec2f NormalizeCoords(double x, double y) const;
	void SetupInitialViewport();
	void SetupGlad();

	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int);
	static void CursorPosCallback(GLFWwindow* window, double x, double y);
	static void FrameBufferSizeCallback(GLFWwindow*, int width, int height);

	Size GetWindowSize() const;
	static GuiApplication* GetInstance(GLFWwindow* window);

	GLFWwindow* m_window;
};
