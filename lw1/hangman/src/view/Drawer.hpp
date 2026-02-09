#pragma once
#include <string>

class Drawer
{
public:
	static void drawLine(float x1, float y1, float x2, float y2);
	static void drawCircle(float cx, float cy, float r, int segments);
	static void drawChar(char c, float x, float y, float size, bool isCorrect, bool isWrong);
};
