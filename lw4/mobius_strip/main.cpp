#include "src/MobiusStripApp.hpp"

int main()
{
	MobiusStripApp app{ 1000, 1000, "The Mobius Strip Visualizer" };

	app.MainLoop();

	return 0;
}