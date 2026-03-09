#include "src/app/AsteroidsApplication.hpp"

#include <iostream>

int main()
{
	AsteroidsApplication asteroids{ 800, 600, "Asteroids" };

	try
	{
		asteroids.MainLoop();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}