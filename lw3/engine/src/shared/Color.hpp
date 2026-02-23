#pragma once
#include <GLFW/glfw3.h>

struct Color
{
	double r = 0, g = 0, b = 0, a = 1;

	void ApplyColor()
	{
		glColor4d(r, g, b, a);
	}
};