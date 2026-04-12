#define GLFW_INCLUDE_NONE
#include "src/app/MemoryTrainerApp.hpp"

int main()
{
	MemoryTrainerApp app{ 1000, 1000, "Memory Trainer" };

	app.MainLoop();

	return 0;
}
