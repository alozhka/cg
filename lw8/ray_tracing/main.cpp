#include "src/RayTracingApp.hpp"

#include <iostream>

int main()
{
	try
	{
		RayTracingApp app(1024, 768, "Ray Tracing");
		app.MainLoop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}