#include "src/RippleApp.hpp"

#include <stb/stb_image.h>

int main()
{
	stbi_set_flip_vertically_on_load(true);
	RippleApp app{ 800, 600, "Ripple transition" };
	app.MainLoop();
	return 0;
}