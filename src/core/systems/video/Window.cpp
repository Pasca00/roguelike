#include "Window.h"

#include <iostream>

WindowConfig::WindowConfig() {
	this->resolveDefaultConfig();
}

WindowConfig::~WindowConfig() { }

void WindowConfig::resolveDefaultConfig() {
	SDL_DisplayMode displayMode;
	SDL_GetDesktopDisplayMode(0, &displayMode);

	float displayRatio = 0.8;

	this->width = displayMode.w * displayRatio;
	this->height = displayMode.h * displayRatio;
	this->posx = SDL_WINDOWPOS_CENTERED;
	this->posy = SDL_WINDOWPOS_CENTERED;
	this->flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
}

Window::Window(std::unique_ptr<WindowConfig>& config) {
	this->config = std::move(config);

	this->sdlWindow = SDL_CreateWindow(
		"Roguelike", 
		this->config->posx,
		this->config->posy, 
		this->config->width, 
		this->config->height, 
		this->config->flags
	);
}

Window::~Window() {
	SDL_DestroyWindow(this->sdlWindow);
}

SDL_Window* Window::getSDLWindow() {
	return this->sdlWindow;
}

int Window::getHeight() {
	return this->config->height;
}

int Window::getWidth() {
	return this->config->width;
}