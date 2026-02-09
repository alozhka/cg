#include "WindowManager.hpp"
#include <iostream>

int main()
{
	WindowManager wm(800, 600, "Hangman Game");
	if (wm.init())
	{
		wm.run();
	}
	else
	{
		std::cerr << "Failed to initialize game" << std::endl;
		return -1;
	}
	return 0;
}
