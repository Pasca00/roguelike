#include <iostream>
#include <SDL.h>

#include "core/Game.h"

int main(int argc, char** argv) {
	Game* game = Game::getInstance();

	while (game->shouldRun()) {
		game->collectInput();
		game->update();
		game->clearScreen();
		game->draw();
	}

	delete game;

	return 0;
}