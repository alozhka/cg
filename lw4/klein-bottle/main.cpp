#include "src/KleinBottleApp.hpp"

int main()
{
	KleinBottleApp app{ 800, 600, "The Klein Bottle Visualizer" };

	app.MainLoop();

	return 0;
}