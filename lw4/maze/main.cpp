#include "src/MazeApp.hpp"

#include <iostream>

int main()
{
	MazeApp app{ 1000, 1000, "The Maze" };

	app.MainLoop();

	return 0;
}
