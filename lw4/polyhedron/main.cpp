#include "src/PolyhedronApp.h"

int main()
{
	PolyhedronApp app{800, 600, "Polyhedron viewer"};

	app.MainLoop();

	return EXIT_SUCCESS;
}