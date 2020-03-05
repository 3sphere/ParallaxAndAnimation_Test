#include <SDL.h>
#include "Game.h"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char** args)
{
	Game game;
	if (game.Init())
	{
		game.Run();
	}
	game.Shutdown();
	return 0;
}