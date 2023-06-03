#include <iostream>
#include <SDL.h>

#include "core/Game.h"

const int SCREEN_FPS = 60;
const float SCREEN_TICKS_PER_FRAME = 1000.0 / SCREEN_FPS;

int main(int argc, char** argv) {
	srand(time(NULL));

	Game* game = Game::getInstance();

	Uint64 start;
	Uint64 end;

	Uint32 nFrames = 0;
	float totalTimeElapsed = 0;

	while (game->shouldRun()) {
		start = SDL_GetPerformanceCounter();

		game->collectInput();
		game->update();
		game->clearScreen();
		game->draw();

		end = SDL_GetPerformanceCounter();

		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.f;
		float delay = SCREEN_TICKS_PER_FRAME - elapsed > 0 ? SCREEN_TICKS_PER_FRAME - elapsed : 0;
		SDL_Delay(floor(delay));

		game->swapWindow();
	}

	delete game;

	return 0;
}