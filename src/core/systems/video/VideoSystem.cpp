#include "VideoSystem.h"

#include <iostream>

void VideoSystem::init() {
	std::cout << "Initializing video system\n";

	this->initSDL();
	this->initWindow();
}

void VideoSystem::initSDL() {
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}

void VideoSystem::initWindow() {
	this->window = std::make_unique<Window>(
		std::make_unique<WindowConfig>()
	);
}

void VideoSystem::initGL() {
	GLenum error = GL_NO_ERROR;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_CreateContext(window->getSDLWindow());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "ERROR ON GLEW INIT " << glewGetErrorString(error) << std::endl;
	}
}

void VideoSystem::initShaders() {}

void VideoSystem::loadInitialTextures() {}