#include "src/ChessApp.hpp"

int main()
{
	ChessApp app(1024, 768, "Chess");
	app.MainLoop();
	return 0;
}