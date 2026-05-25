#include "src/RayTracingApp.hpp"

#include <iostream>

int main()
{
	try
	{
		RayTracingApp app(800, 600);
		app.MainLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}