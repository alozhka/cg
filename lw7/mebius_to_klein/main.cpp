#include "src/MorphingApp.hpp"

#include <iostream>

int main()
{
	MorphingApp app{ 1027, 768, "Mebius strip to Klein bottle" };

	app.MainLoop();

	return 0;
}
