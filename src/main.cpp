#include <iostream>
#include <SDL.h>

#include "core/Game.h"

int main(int argc, char** argv) {
	SDL_Window* window = SDL_CreateWindow("Game", 0, 0, 500, 500, SDL_WINDOW_OPENGL);

	Game* game = Game::getInstance();

	while (game->shouldRun()) {
		game->draw();
	}

	return 0;
}