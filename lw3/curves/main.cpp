#include "src/app/GuiApplication.h"

#include <iostream>

int main()
{
	GuiApplication app(800, 600, "Curves drawer");

	auto onDraw = [] {

	};
	app.MainLoop(onDraw);
}