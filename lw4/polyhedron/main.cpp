#define GLFW_INCLUDE_NONE
#include "src/PolyhedronApp.h"

int main()
{
	PolyhedronApp app{ 800, 800, "Polyhedron viewer" };

	app.MainLoop();

	return EXIT_SUCCESS;
}