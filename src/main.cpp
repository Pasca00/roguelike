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

	Uint64 nFrames = 0;
	float totalTimeElapsed = 0;

	Uint64 endTest;
	float elapsedInput;
	float elapsedUpdate;
	float elapsedDraw;

	while (game->shouldRun()) {
		start = SDL_GetPerformanceCounter();

		game->collectInput();
		//endTest = SDL_GetPerformanceCounter();
		//elapsedInput = (endTest - start) / (float)SDL_GetPerformanceFrequency() * 1000.f;

		game->update();
		//end = SDL_GetPerformanceCounter();
		//elapsedUpdate = (end - endTest) / (float)SDL_GetPerformanceFrequency() * 1000.f;
		//endTest = end;

		game->clearScreen();
		game->draw();
		//end = SDL_GetPerformanceCounter();
		//elapsedDraw = (end - endTest) / (float)SDL_GetPerformanceFrequency() * 1000.f;

		end = SDL_GetPerformanceCounter();

		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.f;
		float delay = SCREEN_TICKS_PER_FRAME - elapsed > 0 ? SCREEN_TICKS_PER_FRAME - elapsed : 0;
		SDL_Delay(floor(delay));

		game->swapWindow();

		totalTimeElapsed += elapsed;
		nFrames++;

		//printf("input: %f, update: %f, draw: %f\n", elapsedInput, elapsedUpdate, elapsedDraw);

		//printf("fps: %f\n", nFrames / (SDL_GetTicks() / 1000.f));
	}

	delete game;

	return 0;
}