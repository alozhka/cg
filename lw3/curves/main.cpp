#define GL_SILENCE_DEPRECATION
#include "src/app/CurveDrawerApplication.hpp"

#include <iostream>
#include <ostream>

int main()
{
	std::setlocale(LC_ALL, "ru_RU.UTF-8");

	try
	{
		CurveDrawerApplication app;
		app.MainLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}