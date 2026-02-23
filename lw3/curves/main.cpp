#include "src/CurveBezier.hpp"
#include "src/app/GuiApplication.h"

#include <iostream>
#define GL_SILENCE_DEPRECATION

int main()
{
	std::setlocale(LC_ALL, "ru_RU.UTF-8");
	CurveBezier bezierCurve{ 100 };
	GuiApplication app(800, 600, "Curves drawer");

	auto onDraw = [&bezierCurve] {
		bezierCurve.Draw();
	};

	app.MainLoop(onDraw);

	return EXIT_SUCCESS;
}