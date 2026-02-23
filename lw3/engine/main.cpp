#include "src/app/EngineApplication.h"

#include <iostream>

int main()
{
	try
	{
		EngineApplication app(800, 600, "Engine Visualizer");
		app.MainLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
